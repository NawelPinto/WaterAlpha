
#include <Arduino.h>

#include "led_rgb.h"
#include "led_colors.h"
#include "config.h"
#include "pins.h"

namespace {

  bool ledReady = false;

  // Primera comprobacion de capacidad de salida.
  // No sustituye la revision del pinout UAI.
  bool candidateOutputPin(int pin) {

    // GPIO 34-39 son solo entradas.
    // GPIO 6-11 estan reservados para Flash
    // en el modulo ESP32 clasico.
    return (
      pin >= 0 &&
      pin <= 33 &&
      !(pin >= 6 && pin <= 11)
    );
  }

  bool wiringAllowed() {

    if (
      !WAPins::PINOUT_CONFIRMED ||
      !WAPins::LED_TYPE_CONFIRMED
    ) {
      return false;
    }

    int r = WAPins::PIN_LED_R;
    int g = WAPins::PIN_LED_G;
    int b = WAPins::PIN_LED_B;

    // Los tres pines deben ser distintos.
    if (r == g || r == b || g == b) {
      return false;
    }

    return (
      candidateOutputPin(r) &&
      candidateOutputPin(g) &&
      candidateOutputPin(b)
    );
  }

  uint32_t pwmDuty(bool enabled) {

    uint32_t value =
      enabled ? WAConfig::LED_BRIGHTNESS : 0;

    // Invertir para anodo comun.
    if (WAPins::LED_COMMON_ANODE) {
      value = 255 - value;
    }

    return value;
  }

  void writeChannel(int pin, bool enabled) {

    ledcWrite(
      static_cast<uint8_t>(pin),
      pwmDuty(enabled)
    );
  }

}

namespace WALed {

  void begin() {

    ledReady = false;

    if (!WAConfig::USE_REAL_LED) {
      Serial.println("[LED] Modo MOCK");
      return;
    }

    if (!wiringAllowed()) {

      Serial.println(
        "[LED] ERROR: cableado no confirmado"
      );

      return;
    }

    const uint32_t frequency =
      WAConfig::LED_PWM_FREQUENCY;

    const uint8_t resolution =
      WAConfig::LED_PWM_BITS;

    // Asociar pines a canales PWM.
    bool okR = ledcAttach(
      WAPins::PIN_LED_R,
      frequency,
      resolution
    );

    bool okG = ledcAttach(
      WAPins::PIN_LED_G,
      frequency,
      resolution
    );

    bool okB = ledcAttach(
      WAPins::PIN_LED_B,
      frequency,
      resolution
    );

    if (!okR || !okG || !okB) {

      Serial.println(
        "[LED] ERROR: no se pudo iniciar PWM"
      );

      // Desactivar y liberar los canales
      // que hayan logrado inicializarse.
      const uint32_t offDuty = pwmDuty(false);

      if (okR) {
        ledcWrite(WAPins::PIN_LED_R, offDuty);
        ledcDetach(WAPins::PIN_LED_R);
      }

      if (okG) {
        ledcWrite(WAPins::PIN_LED_G, offDuty);
        ledcDetach(WAPins::PIN_LED_G);
      }

      if (okB) {
        ledcWrite(WAPins::PIN_LED_B, offDuty);
        ledcDetach(WAPins::PIN_LED_B);
      }

      return;
    }

    ledReady = true;

    // Comenzar con todos los canales apagados.
    writeChannel(WAPins::PIN_LED_R, false);
    writeChannel(WAPins::PIN_LED_G, false);
    writeChannel(WAPins::PIN_LED_B, false);

    Serial.println("[LED] RGB inicializado");
  }


  void setState(WA::State state) {

    // Nunca escribir sobre pines no habilitados.
    if (!ledReady) {
      return;
    }

    WAColors::Channels color =
      WAColors::fromState(state);

    writeChannel(
      WAPins::PIN_LED_R,
      color.red
    );

    writeChannel(
      WAPins::PIN_LED_G,
      color.green
    );

    writeChannel(
      WAPins::PIN_LED_B,
      color.blue
    );
  }

}
