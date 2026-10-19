
#include <Arduino.h>

#include "controller.h"
#include "mock_sensors.h"
#include "state_machine.h"
#include "config.h"

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

  // Actuador simulado
  void setLedState(WA::State state) {

    Serial.print("[LED MOCK] ");
    Serial.println(WA::stateName(state));
  }

}

namespace WAController {

  void begin() {

    bootTime = millis();
    lastSampleTime = bootTime;

    machine.reset();

    currentReadings = WAMock::read(0);

    currentDecision =
      machine.evaluate(currentReadings);

    initialized = true;

    Serial.println("[CTRL] Controlador iniciado");

    setLedState(currentDecision.state);
  }

  void update() {

    if (!initialized) return;

    uint32_t now = millis();

    // Esperar hasta el siguiente muestreo
    if (now - lastSampleTime <
        WAConfig::SAMPLE_PERIOD_MS) {
      return;
    }

    lastSampleTime = now;

    // Obtener nuevas mediciones
    currentReadings =
      WAMock::read(now - bootTime);

    // Calcular estado una sola vez
    WA::Decision next =
      machine.evaluate(currentReadings);

    // Registrar solamente cambios de estado
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

}
