#include <Arduino.h>
#include "led_rgb.h"
#include "led_status_logic.h"
#include "config.h"
#include "pins.h"

namespace {
 bool ready = false;
 WA::State currentState = WA::State::BLUE;
 // No admitir GPIO de arranque, Flash, serie ni pines solo-entrada.
 bool candidateOutputPin(int pin) {
  switch(pin) {
   case 13: case 14: case 18: case 19: case 21: case 22: case 23:
   case 25: case 26: case 27: case 32: case 33: return true;
   default: return false;
  }
 }
 bool wiringAllowed() {
  using namespace WAPins;
  if (!PINOUT_CONFIRMED || !LED_PINS_CONFIRMED ||
      !LED_CURRENT_LIMITING_CONFIRMED || !LED_POLARITY_CONFIRMED) return false;
  const int r = PIN_LED_RED, g = PIN_LED_GREEN, y = PIN_LED_YELLOW;
  if (!candidateOutputPin(r) || !candidateOutputPin(g) || !candidateOutputPin(y)) return false;
  if (r==g || r==y || g==y) return false;
  if (r==PIN_TEMP || g==PIN_TEMP || y==PIN_TEMP) return false;
  if (r==PIN_PH_ADC || g==PIN_PH_ADC || y==PIN_PH_ADC) return false;
  // CONFIRMAR ademas disponibilidad de estos pines en la placa UAI.
  return true;
 }
}
namespace WALed {
 void begin() {
  ready = false;
  if (!WAConfig::USE_REAL_LED) {
    Serial.println("[LED] Inactivo hasta confirmar 3 resistencias, pines y polaridad");
    return;
  }
  if (!wiringAllowed()) {
    Serial.println("[LED] BLOQUEADO: uno o mas requisitos electricos sin validar");
    return;
  }
  const int pins[] = {WAPins::PIN_LED_RED, WAPins::PIN_LED_GREEN, WAPins::PIN_LED_YELLOW};
  for (const int pin: pins) { digitalWrite(pin,LOW); pinMode(pin,OUTPUT); }
  ready = true;
  Serial.println("[LED] Tres indicadores discretos habilitados");
  update();
 }
 void setState(WA::State state) { currentState = state; update(); }
 void update() {
  if (!ready) return;
  WALedLogic::Output o = WALedLogic::fromState(currentState,millis());
  digitalWrite(WAPins::PIN_LED_RED, o.red ? HIGH : LOW);
  digitalWrite(WAPins::PIN_LED_GREEN, o.green ? HIGH : LOW);
  digitalWrite(WAPins::PIN_LED_YELLOW, o.yellow ? HIGH : LOW);
 }
}
