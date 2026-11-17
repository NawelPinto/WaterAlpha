/* WaterAlpha Avance 3: sensor real + SoftAP + servidor LittleFS + 3 LED discretos opcionales. */
#include <Arduino.h>
#include "config.h"
#include "controller.h"
#include "web_server.h"
#include "led_rgb.h"

void setup() {
  Serial.begin(WAConfig::SERIAL_BAUD);
  Serial.println("\n[BOOT] WaterAlpha Avance 3");
  WAController::begin();
  startWaterAlphaServer(); // Inicia SoftAP solo una vez, aqui a traves del servidor
}

void loop() {
  WAController::update();
  WALed::update();  // Blink sin delay bloqueante
  updateWaterAlphaServer();
  delay(2);
}
