
/*
 * WATERALPHA
 * Firmware Alpha v0.1
 *
 * ESP32 clásico
 * Wi-Fi SoftAP + servidor HTTP
 * Sensores simulados
 */

 #include "controller.h"
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "web_server.h"

bool serverReady = false;

// Se ejecuta una vez al iniciar
void setup() {

  Serial.begin(WAConfig::SERIAL_BAUD);

  Serial.println();
  Serial.println("[BOOT] WaterAlpha v0.1");
  WAController::begin();

  // Activar modo punto de acceso
  WiFi.mode(WIFI_AP);

  // Crear nuestra propia red Wi-Fi
  bool wifiOK = WiFi.softAP(
    WAConfig::WIFI_SSID,
    WAConfig::WIFI_PASSWORD
  );

  if (!wifiOK) {
    Serial.println("[ERROR] No se pudo iniciar SoftAP");
    return;
  }

  Serial.println("[WIFI] SoftAP iniciado");

  Serial.print("[WIFI] Nombre: ");
  Serial.println(WAConfig::WIFI_SSID);

  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.softAPIP());

  // Iniciar servidor HTTP
  startWaterAlphaServer();

  serverReady = true;

  Serial.println("[SYSTEM] WaterAlpha listo");
}

// Se ejecuta repetidamente

void loop() {

  // Actualizar mediciones y máquina de estados
  WAController::update();

  // Atender solicitudes HTTP
  if (serverReady) {
    updateWaterAlphaServer();
  }

  delay(2);
}

