
#include <Arduino.h>

#include "config.h"
#include "pins.h"
#include "ph_sensor.h"

namespace {

  bool adcReady = false;

  // ADC1 del ESP32 clasico: GPIO 32 a 39.
  // No todos estan necesariamente
  // expuestos en nuestra placa UAI.
  bool isAdc1Pin(int pin) {

    return pin >= 32 && pin <= 39;
  }

}

namespace WAPh {

  void begin() {

    adcReady = false;

    // Mantener modo simulado
    if (!WAConfig::USE_REAL_PH) {

      Serial.println("[PH] Modo MOCK");
      return;
    }

    // Verificar pinout
    if (
      !WAPins::PINOUT_CONFIRMED ||
      WAPins::PIN_PH_ADC < 0
    ) {

      Serial.println(
        "[PH] ERROR: pinout no confirmado"
      );

      return;
    }

    // Verificar que utilicemos ADC1
    if (!isAdc1Pin(WAPins::PIN_PH_ADC)) {

      Serial.println(
        "[PH] ERROR: GPIO fuera de ADC1"
      );

      return;
    }

    // Configurar adquisicion
    analogReadResolution(12);

    analogSetPinAttenuation(
      static_cast<uint8_t>(WAPins::PIN_PH_ADC),
      ADC_11db
    );

    adcReady = true;

    Serial.println("[PH] ADC1 configurado");
    Serial.println(
      "[PH] Calibracion de pH pendiente"
    );
  }

  RawReading read() {

    // No utilizar ningun pin sin autorizacion
    if (!adcReady) {

      return {
        0,
        false
      };
    }

    uint32_t millivolts =
      analogReadMilliVolts(
        static_cast<uint8_t>(WAPins::PIN_PH_ADC)
      );

    return {
      millivolts,
      true
    };
  }

}
