
#include <Arduino.h>
#include <limits>

#include "controller.h"
#include "mock_sensors.h"
#include "state_machine.h"
#include "config.h"
#include "temperature_sensor.h"
#include "ph_sensor.h"

namespace {

  WA::StateMachine machine;

  WA::Readings currentReadings{};

  WA::Decision currentDecision{
    WA::State::BLUE,
    "Inicializando.",
    false
  };

  uint32_t bootTime = 0;
  uint32_t lastSampleTime = 0;

  bool initialized = false;


  // Actuador LED simulado
  void setLedState(WA::State state) {

    Serial.print("[LED MOCK] ");
    Serial.println(WA::stateName(state));
  }


  // NUEVA FUNCION: FUENTE DE PH
  void applyPhSource(WA::Readings& r) {

    // Mantener el pH simulado
    if (!WAConfig::USE_REAL_PH) {
      return;
    }

    // Obtener voltaje desde ADC1
    WAPh::RawReading raw = WAPh::read();

    // Todavia no tenemos calibracion real
    r.ph =
      std::numeric_limits<float>::quiet_NaN();

    r.ph_sensor_ok = false;
    r.calibrated = false;

    if (raw.acquired) {

      Serial.print("[PH ADC] ");
      Serial.print(raw.adcMillivolts);
      Serial.println(" mV - sin calibrar");

    } else {

      Serial.println(
        "[PH ADC] Adquisicion no disponible"
      );
    }
  }

} // FIN DEL NAMESPACE ANONIMO


namespace WAController {

  void begin() {

    bootTime = millis();
    lastSampleTime = bootTime;

    machine.reset();

    currentReadings = WAMock::read(0);

    // Inicializar sensor de temperatura
    WATemperature::begin();

    // Inicializar sensor de pH
    WAPh::begin();

    // Aplicar temperatura real si corresponde
    if (WAConfig::USE_REAL_TEMPERATURE) {

      WATemperature::Reading temp =
        WATemperature::read();

      currentReadings.temperature = temp.celsius;

      currentReadings.temperature_sensor_ok =
        temp.sensor_ok;
    }

    // Aplicar fuente de pH
    applyPhSource(currentReadings);

    // Evaluar estado inicial
    currentDecision =
      machine.evaluate(currentReadings);

    initialized = true;

    Serial.println("[CTRL] Controlador iniciado");

    setLedState(currentDecision.state);
  }


  void update() {

    if (!initialized) return;

    uint32_t now = millis();

    // Esperar siguiente muestreo
    if (now - lastSampleTime <
        WAConfig::SAMPLE_PERIOD_MS) {
      return;
    }

    lastSampleTime = now;

    // Obtener mediciones simuladas
    currentReadings =
      WAMock::read(now - bootTime);

    // Reemplazar temperatura si es real
    if (WAConfig::USE_REAL_TEMPERATURE) {

      WATemperature::Reading temp =
        WATemperature::read();

      currentReadings.temperature = temp.celsius;

      currentReadings.temperature_sensor_ok =
        temp.sensor_ok;
    }

    // Reemplazar pH si corresponde
    applyPhSource(currentReadings);

    // Calcular estado una sola vez
    WA::Decision next =
      machine.evaluate(currentReadings);

    // Registrar cambios de estado
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

} // FIN DE WAController
