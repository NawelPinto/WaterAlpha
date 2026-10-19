
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <cmath>
#include <limits>

#include "config.h"
#include "pins.h"
#include "temperature_sensor.h"

namespace {

  // Se crean los objetos sin asignar GPIO.
  OneWire oneWire;
  DallasTemperature sensors(&oneWire);

  bool sensorEnabled = false;

  uint32_t lastRequestMs = 0;

  WATemperature::Reading lastReading{
    std::numeric_limits<float>::quiet_NaN(),
    false
  };

  WATemperature::Reading invalidReading() {
    return {
      std::numeric_limits<float>::quiet_NaN(),
      false
    };
  }

}

namespace WATemperature {

  void begin() {

    sensorEnabled = false;
    lastReading = invalidReading();

    // Mantener la simulacion activa.
    if (!WAConfig::USE_REAL_TEMPERATURE) {
      Serial.println("[TEMP] Modo MOCK");
      return;
    }

    // Nunca utilizar GPIO sin verificar el pinout.
    if (
      !WAPins::PINOUT_CONFIRMED ||
      WAPins::PIN_TEMP < 0
    ) {
      Serial.println(
        "[TEMP] ERROR: GPIO no confirmado"
      );
      return;
    }

    // Configurar comunicacion 1-Wire.
    oneWire.begin(
      static_cast<uint8_t>(WAPins::PIN_TEMP)
    );

    sensors.begin();

    // 12 bits: hasta 750 ms por conversion.
    sensors.setResolution(12);

    // No bloquear el procesamiento.
    sensors.setWaitForConversion(false);

    sensorEnabled = true;

    // Solicitar primera medicion.
    sensors.requestTemperatures();
    lastRequestMs = millis();

    Serial.println("[TEMP] DS18B20 inicializado");
  }

  Reading read() {

    if (!sensorEnabled) {
      return invalidReading();
    }

    // Esperar sin bloquear hasta que termine
    // el tiempo de conversion.
    if (
      uint32_t(millis() - lastRequestMs) <
      WAConfig::TEMP_CONVERSION_MS
    ) {
      return lastReading;
    }

    float value = sensors.getTempCByIndex(0);

    bool valid =
      std::isfinite(value) &&
      value != DEVICE_DISCONNECTED_C &&
      value >= WAConfig::TEMP_SENSOR_MIN &&
      value <= WAConfig::TEMP_SENSOR_MAX;

    if (valid) {

      lastReading = {
        value,
        true
      };

    } else {

      lastReading = invalidReading();

      // Volver a buscar sensores por si
      // alguno se desconecto o reconecto.
      sensors.begin();

      Serial.println(
        "[TEMP] Lectura invalida o sensor ausente"
      );
    }

    // Iniciar conversion siguiente.
    sensors.requestTemperatures();
    lastRequestMs = millis();

    return lastReading;
  }

}
