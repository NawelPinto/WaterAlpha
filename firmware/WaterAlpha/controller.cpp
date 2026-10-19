#include <Arduino.h>
#include <cmath>
#include <limits>
#include <stdint.h>

#include "controller.h"
#include "config.h"
#include "mock_sensors.h"
#include "state_machine.h"
#include "temperature_sensor.h"
#include "ph_sensor.h"
#include "ph_calibration.h"
#include "ph_pipeline.h"
#include "ph_field_data.h"
#include "led_rgb.h"

namespace {

  WA::StateMachine machine;
  WA::Readings currentReadings{};
  WA::Decision currentDecision{
    WA::State::BLUE, "Inicializando.", false
  };

  uint32_t bootTime = 0;
  uint32_t lastSampleTime = 0;
  bool initialized = false;

  WAPhCalibration::Calibration phModel{0.0, 0.0, false};
  bool phCalibrationVerified = false;

  // Ventana corta para calcular estabilidad del voltaje de pH REAL.
  // No usar datos MOCK aqui.
  constexpr uint8_t WINDOW_CAPACITY = 5;
  double voltageWindow[WINDOW_CAPACITY]{};
  uint8_t windowCount = 0;
  uint8_t windowNext = 0;

  float invalidFloat() {
    return std::numeric_limits<float>::quiet_NaN();
  }

  void clearVoltageWindow() {
    windowCount = 0;
    windowNext = 0;
  }

  bool updateVoltageStability(double adcMillivolts) {
    if (
      !std::isfinite(adcMillivolts) ||
      WAField::STABILITY_MAX_SPAN_MV <= 0.0 ||
      WAField::STABILITY_SAMPLES != WINDOW_CAPACITY
    ) {
      clearVoltageWindow();
      return false;
    }

    voltageWindow[windowNext] = adcMillivolts;
    windowNext = (windowNext + 1) % WINDOW_CAPACITY;
    if (windowCount < WINDOW_CAPACITY) {
      ++windowCount;
    }
    if (windowCount < WINDOW_CAPACITY) {
      return false;
    }

    double minimum = voltageWindow[0];
    double maximum = voltageWindow[0];
    for (uint8_t i = 1; i < WINDOW_CAPACITY; ++i) {
      if (voltageWindow[i] < minimum) minimum = voltageWindow[i];
      if (voltageWindow[i] > maximum) maximum = voltageWindow[i];
    }
    return (maximum - minimum) <= WAField::STABILITY_MAX_SPAN_MV;
  }

  bool adcInVerifiedRange(double value) {
    if (!std::isfinite(value)) return false;
    if (WAField::ADC_VALID_MIN_MV < 0.0) return false;
    if (WAField::ADC_VALID_MAX_MV <= WAField::ADC_VALID_MIN_MV) {
      return false;
    }
    return (
      value >= WAField::ADC_VALID_MIN_MV &&
      value <= WAField::ADC_VALID_MAX_MV
    );
  }

  // Dos puntos generan el modelo. El tercero comprueba su error.
  // Todo queda INVALIDO mientras falten datos o verificacion fisica.
  void loadFieldCalibration() {
    phModel = WAPhCalibration::fitTwoPoint(
      {WAField::BUFFER_1_ADC_MV, WAField::BUFFER_1_PH},
      {WAField::BUFFER_2_ADC_MV, WAField::BUFFER_2_PH}
    );
    phCalibrationVerified = false;

    if (!phModel.valid) {
      Serial.println("[PH CAL] Faltan puntos validos de calibracion");
      return;
    }

    if (
      !WAField::ELECTRICAL_CHAIN_VERIFIED ||
      !WAField::PROBE_RESPONSE_VERIFIED ||
      !WAField::CALIBRATION_REVIEWED ||
      WAField::MAX_CHECK_ERROR_PH <= 0.0 ||
      !adcInVerifiedRange(WAField::CHECK_BUFFER_ADC_MV) ||
      !adcInVerifiedRange(WAField::BUFFER_1_ADC_MV) ||
      !adcInVerifiedRange(WAField::BUFFER_2_ADC_MV)
    ) {
      Serial.println("[PH CAL] Calibracion pendiente de verificacion");
      return;
    }

    WAPhCalibration::Result checked = WAPhCalibration::estimatePh(
      phModel, WAField::CHECK_BUFFER_ADC_MV
    );

    if (!checked.valid) {
      Serial.println("[PH CAL] Fallo la verificacion matematica");
      return;
    }

    double error = std::fabs(checked.ph - WAField::CHECK_BUFFER_PH);
    phCalibrationVerified = (error <= WAField::MAX_CHECK_ERROR_PH);

    Serial.print("[PH CAL] Error del patron de comprobacion: ");
    Serial.print(error, 3);
    Serial.println(" pH");
    Serial.println(phCalibrationVerified
      ? "[PH CAL] Verificacion registrada: OK"
      : "[PH CAL] Verificacion rechazada");
  }

  void setLedState(WA::State state) {
    Serial.print(WAConfig::USE_REAL_LED ? "[LED FISICO] " : "[LED VIRTUAL] ");
    Serial.println(WA::stateName(state));
    WALed::setState(state); // Sin GPIO si USE_REAL_LED es false.
  }

  WA::Readings readRealSensors(uint32_t now) {
    WA::Readings r{};
    r.ph = invalidFloat();
    r.temperature = invalidFloat();
    r.ph_sensor_ok = false;
    r.temperature_sensor_ok = false;
    r.calibrated = false;
    r.stable = false;
    r.initializing = (uint32_t(now - bootTime) < WAField::REAL_STARTUP_MS);

    // Temperatura: solo lecturas obtenidas realmente por 1-Wire.
    if (WAConfig::USE_REAL_TEMPERATURE) {
      WATemperature::Reading temp = WATemperature::read();
      r.temperature = temp.celsius;
      r.temperature_sensor_ok = temp.sensor_ok;
    }

    // pH: ADC real -> calibracion -> validacion -> estabilidad.
    if (WAConfig::USE_REAL_PH) {
      WAPh::RawReading raw = WAPh::read();
      if (raw.acquired) {
        Serial.print("[PH ADC REAL] ");
        Serial.print(raw.adcMillivolts);
        Serial.println(" mV");
      } else {
        Serial.println("[PH ADC REAL] Adquisicion no disponible");
      }

      const double volts = static_cast<double>(raw.adcMillivolts);
      const bool voltagePlausible = raw.acquired && adcInVerifiedRange(volts);
      const bool signalEvidence = (
        voltagePlausible &&
        WAField::ELECTRICAL_CHAIN_VERIFIED &&
        WAField::PROBE_RESPONSE_VERIFIED
      );

      WAPhPipeline::Reading ph = WAPhPipeline::process(
        volts,
        raw.acquired,
        phModel,
        {signalEvidence, phCalibrationVerified}
      );

      r.ph = ph.ph;
      r.ph_sensor_ok = ph.ph_sensor_ok;
      r.calibrated = ph.calibrated;

      if (r.ph_sensor_ok && r.temperature_sensor_ok) {
        r.stable = updateVoltageStability(volts);
      } else {
        clearVoltageWindow();
      }
    } else {
      clearVoltageWindow();
    }
    return r;
  }

  WA::Readings takeReading(uint32_t now) {
    if (WAConfig::MOCK_MODE) {
      return WAMock::read(now - bootTime);
    }
    return readRealSensors(now);
  }

} // namespace anonimo

namespace WAController {

  void begin() {
    bootTime = millis();
    lastSampleTime = bootTime;
    machine.reset();
    clearVoltageWindow();

    if (WAConfig::MOCK_MODE) {
      Serial.println("[CTRL] Modo MOCK: ningun sensor fisico habilitado");
    } else {
      Serial.println("[CTRL] Modo REAL: nunca se mezclan datos MOCK");
      if (WAConfig::USE_REAL_TEMPERATURE) WATemperature::begin();
      if (WAConfig::USE_REAL_PH) {
        WAPh::begin();
        loadFieldCalibration();
      }
    }

    WALed::begin();

    currentReadings = takeReading(bootTime);
    currentDecision = machine.evaluate(currentReadings);
    initialized = true;
    Serial.println("[CTRL] Controlador iniciado");
    setLedState(currentDecision.state);
  }

  void update() {
    if (!initialized) return;
    uint32_t now = millis();
    if (uint32_t(now - lastSampleTime) < WAConfig::SAMPLE_PERIOD_MS) return;
    lastSampleTime = now;

    currentReadings = takeReading(now);
    WA::Decision next = machine.evaluate(currentReadings);
    if (next.state != currentDecision.state) {
      Serial.print("[STATE] ");
      Serial.println(WA::stateName(next.state));
      setLedState(next.state);
    }
    currentDecision = next;
  }

  Snapshot snapshot() {
    return {
      currentReadings,
      currentDecision,
      uint32_t(millis() - lastSampleTime)
    };
  }

} // namespace WAController
