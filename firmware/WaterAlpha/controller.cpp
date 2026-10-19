#include "ph_calibration.h"
#include "ph_pipeline.h"
#include <Arduino.h>
#include <limits>
#include "led_rgb.h"
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

    // Mantener diagnostico por Serial.
    Serial.print("[LED MOCK] ");
    Serial.println(WA::stateName(state));

    // Actualizar LED fisico si esta habilitado.
    WALed::setState(state);
  }



  
  // FUENTE Y VALIDACION DE PH
  void applyPhSource(WA::Readings& r) {

    // En modo MOCK mantenemos los valores
    // generados por mock_sensors.cpp.
    if (!WAConfig::USE_REAL_PH) {
      return;
    }

    // Obtener voltaje desde el ADC.
    WAPh::RawReading raw = WAPh::read();

    // Todavia NO existe un modelo de
    // calibracion obtenido del sensor real.
    const WAPhCalibration::Calibration model{
      0.0,
      0.0,
      false
    };

    // No tenemos validacion fisica.
    // NO cambiar estos valores a true
    // sin completar las pruebas reales.
    const WAPhPipeline::Evidence evidence{
      false,  // Senal no validada
      false   // Calibracion no verificada
    };

    // Procesar la muestra.
    WAPhPipeline::Reading result =
      WAPhPipeline::process(
        static_cast<double>(raw.adcMillivolts),
        raw.acquired,
        model,
        evidence
      );

    // Actualizar datos del controlador.
    r.ph = result.ph;
    r.ph_sensor_ok = result.ph_sensor_ok;
    r.calibrated = result.calibrated;

    // Diagnostico para desarrollo.
    if (raw.acquired) {

      Serial.print("[PH ADC] ");
      Serial.print(raw.adcMillivolts);
      Serial.println(" mV");

    } else {

      Serial.println(
        "[PH ADC] Lectura no disponible"
      );
    }

    if (!result.calibrated) {

      Serial.println(
        "[PH] Medicion no validada"
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

    // Inicializar controlador LED RGB
    WALed::begin();


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
