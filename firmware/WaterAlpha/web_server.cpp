#include <LittleFS.h>
#include <Arduino.h>
#include <WebServer.h>

#include "config.h"
#include "web_server.h"

// Servidor HTTP en el puerto configurado
WebServer server(WAConfig::HTTP_PORT);

bool filesystemReady = false;

void serveStaticFile(
  const char* path,
  const char* contentType
) {

  if (!filesystemReady) {
    server.send(
      503,
      "text/plain",
      "Sistema de archivos no disponible"
    );
    return;
  }

  File file = LittleFS.open(path, "r");

  if (!file || file.isDirectory()) {
    server.send(
      404,
      "text/plain",
      "Archivo no encontrado"
    );
    return;
  }

  server.streamFile(file, contentType);
  file.close();
}

// Página temporal de diagnóstico
void handleRoot() {

  const char* html = R"html(
    <!DOCTYPE html>
    <html lang="es">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport"
            content="width=device-width, initial-scale=1">
      <title>WaterAlpha ESP32</title>
    </head>
    <body>
      <h1>WaterAlpha - ESP32</h1>
      <p>Servidor HTTP funcionando.</p>
      <p>Datos simulados de desarrollo.</p>
      <a href="/api/readings">Ver API JSON</a>
    </body>
    </html>
  )html";

  server.send(
    200,
    "text/html; charset=utf-8",
    html
  );
}

// Endpoint de mediciones simuladas
void handleReadings() {

  const char* json = R"json(
{
  "schema_version": 1,
  "mode": "MOCK",
  "temperature": 17.8,
  "ph": 7.2,
  "state": "GREEN",
  "stable": true,
  "temperature_sensor_ok": true,
  "ph_sensor_ok": true,
  "calibrated": true,
  "reading_valid": true,
  "sample_age_ms": 0,
  "message": "El pH esta dentro de la referencia preliminar.",
  "warning": "Evaluacion preliminar. No certifica potabilidad ni ausencia de patogenos."
}
  )json";

  server.send(
    200,
    "application/json; charset=utf-8",
    json
  );
}

// Activar rutas y servidor
void startWaterAlphaServer() {

    
filesystemReady = LittleFS.begin(false);

if (filesystemReady) {
  Serial.println("[FS] LittleFS montado");
} else {
  Serial.println("[FS] LittleFS no disponible");
}

// Página principal
server.on("/", HTTP_GET, []() {
  serveStaticFile("/index.html", "text/html");
});

// Hoja de estilos
server.on("/style.css", HTTP_GET, []() {
  serveStaticFile("/style.css", "text/css");
});

// Archivos JavaScript
const char* scripts[] = {
  "/config.js",
  "/mock.js",
  "/state_machine.js",
  "/api.js",
  "/app.js"
};

for (const char* path : scripts) {
  server.on(path, HTTP_GET, [path]() {
    serveStaticFile(path, "text/javascript");
  });
}


  server.on(
    "/api/readings",
    HTTP_GET,
    handleReadings
  );

  server.onNotFound([]() {
    server.send(
      404,
      "text/plain",
      "Ruta no encontrada"
    );
  });

  server.begin();

  Serial.println("[HTTP] Servidor iniciado");
}

// Atender solicitudes HTTP
void updateWaterAlphaServer() {
  server.handleClient();
}
