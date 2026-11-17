// WaterAlpha - servidor HTTP local y API JSON v1
// Variante de integracion: voltaje ADC REAL y pH estimado por calibracion experimental.
// Conserva las firmas del proyecto: startWaterAlphaServer / updateWaterAlphaServer.

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <FS.h>
#include <cmath>
#include <stdint.h>

#include "config.h"
#include "controller.h"
#include "state_machine.h"
#include "screening_rules.h"
#include "ph_field_data.h"

namespace {

WebServer server(WAConfig::HTTP_PORT);
bool filesystemReady = false;

// JSON escaping evita romper la respuesta si un mensaje contiene comillas.
String jsonEscape(const String &source) {
  String result;
  result.reserve(source.length() + 8);

  for (size_t i = 0; i < source.length(); ++i) {
    const char c = source[i];
    switch (c) {
      case '"': result += "\\\""; break;
      case '\\': result += "\\\\"; break;
      case '\n': result += "\\n"; break;
      case '\r': result += "\\r"; break;
      case '\t': result += "\\t"; break;
      default:
        if (static_cast<unsigned char>(c) < 32) {
          result += ' ';
        } else {
          result += c;
        }
        break;
    }
  }
  return result;
}

String jsonNumber(double value, uint8_t decimals) {
  if (!std::isfinite(value)) return "null";
  return String(value, static_cast<unsigned int>(decimals));
}

const char *jsonBool(bool value) {
  return value ? "true" : "false";
}

// Acepta los nombres de edad de muestra usados por revisiones del controlador.
template <typename T>
auto sampleAgeMs(const T &s, int) -> decltype(s.sample_age_ms) {
  return s.sample_age_ms;
}
template <typename T>
auto sampleAgeMs(const T &s, long) -> decltype(s.sampleAgeMs) {
  return s.sampleAgeMs;
}
template <typename T>
auto sampleAgeMs(const T &s, char) -> decltype(s.age_ms) {
  return s.age_ms;
}
template <typename T>
uint32_t sampleAgeMs(const T &, ...) {
  return static_cast<uint32_t>(-1); // Sin edad => dato no demostrablemente reciente.
}

void serveStaticFile(const char *path, const char *contentType) {
  if (!filesystemReady) {
    server.send(503, "text/plain; charset=utf-8", "LittleFS no disponible");
    return;
  }

  File file = LittleFS.open(path, "r");
  if (!file || file.isDirectory()) {
    if (file) file.close();
    server.send(404, "text/plain; charset=utf-8", "Archivo no encontrado");
    return;
  }

  server.sendHeader("Cache-Control", "no-store, max-age=0");
  server.streamFile(file, contentType);
  file.close();
}

void handleRoot() {
  serveStaticFile("/index.html", "text/html; charset=utf-8");
}

void handleReadings() {
  const WAController::Snapshot s = WAController::snapshot();
  const auto &r = s.readings;
  const auto &d = s.decision;

  const uint32_t ageMs = static_cast<uint32_t>(sampleAgeMs(s, 0));

  // Este campo es oficial y debe permanecer FALSE si falla la validacion.
  const bool readingValid =
      !r.initializing && r.temperature_sensor_ok && r.ph_sensor_ok &&
      r.calibrated && r.stable &&
      std::isfinite(static_cast<double>(r.temperature)) &&
      std::isfinite(static_cast<double>(r.ph)) &&
      ageMs <= WAScreening::MAX_AGE_MS;

  const double adcMv = WAConfig::MOCK_MODE
      ? NAN : WAController::phAdcMillivolts();
  const double experimentalPh = WAConfig::MOCK_MODE
      ? NAN : WAController::phEstimatedPreview();
  // Tamizaje experimental independiente de la validez formal del estado.
  // Nunca utilizar PH_DENTRO_REFERENCIA como equivalente a GREEN/potabilidad.
  const bool screeningEvidence = WAField::ELECTRICAL_CHAIN_VERIFIED &&
      WAField::PROBE_RESPONSE_VERIFIED;
  const WAScreening::Result screening = WAScreening::evaluate(
      !WAConfig::MOCK_MODE,
      r.temperature_sensor_ok,
      static_cast<double>(r.temperature),
      screeningEvidence ? experimentalPh : NAN,
      ageMs
  );

  String json;
  json.reserve(640);
  json += "{\"schema_version\":1,";
  json += "\"mode\":\"";
  json += (WAConfig::MOCK_MODE ? "MOCK" : "REAL");
  json += "\",";

  json += "\"temperature\":";
  json += r.temperature_sensor_ok
      ? jsonNumber(r.temperature, 2) : String("null");
  json += ",\"ph\":";
  json += r.ph_sensor_ok && r.calibrated
      ? jsonNumber(r.ph, 2) : String("null");

  // Datos diagnosticos independientes de la validez metrologica.
  json += ",\"ph_adc_mv\":";
  json += jsonNumber(adcMv, 0);
  json += ",\"ph_estimated\":";
  json += jsonNumber(experimentalPh, 2);

  // Diagnostico adicional: nunca declara el agua potable.
  json += ",\"potability\":\"INDETERMINADA\"";
  json += ",\"calibration_check_ref_ph\":7.09";
  json += ",\"calibration_check_adc_mv\":2113.0";
  json += ",\"calibration_observed_abs_error_ph\":0.316";
  json += ",\"screening_status\":\"";
  json += jsonEscape(String(screening.status));
  json += "\",\"screening_message\":\"";
  json += jsonEscape(String(screening.message));
  json += "\",\"ph_reference_status\":\"";
  json += jsonEscape(String(screening.phBand));
  json += "\",\"temperature_reference_status\":\"";
  json += jsonEscape(String(screening.tempBand));
  json += "\",\"ph_reference_min\":6.5,\"ph_reference_max\":8.5";
  json += ",\"temp_calibration_c\":20.9,\"temp_calibration_tolerance_c\":2.0";

  json += ",\"state\":\"";
  json += jsonEscape(String(WA::stateName(d.state)));
  json += "\",\"stable\":";
  json += jsonBool(r.stable);
  json += ",\"temperature_sensor_ok\":";
  json += jsonBool(r.temperature_sensor_ok);
  json += ",\"ph_sensor_ok\":";
  json += jsonBool(r.ph_sensor_ok);
  json += ",\"calibrated\":";
  json += jsonBool(r.calibrated);
  json += ",\"reading_valid\":";
  json += jsonBool(readingValid);
  json += ",\"sample_age_ms\":";
  json += String(ageMs);
  json += ",\"message\":\"";
  json += jsonEscape(String(d.message));
  json += "\",\"warning\":\"";
  json += "Monitoreo instrumental de pH y temperatura. ";
  json += "Estimacion experimental independiente de la validacion metrologica. ";
  json += "No certifica potabilidad ni ausencia de patogenos.";
  json += "\"}";

  server.sendHeader("Cache-Control", "no-store, max-age=0");
  server.send(200, "application/json; charset=utf-8", json);
}

void registerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/index.html", HTTP_GET, handleRoot);
  server.on("/api/readings", HTTP_GET, handleReadings);

  server.on("/style.css", HTTP_GET, []() {
    serveStaticFile("/style.css", "text/css; charset=utf-8");
  });
  server.on("/config.js", HTTP_GET, []() {
    serveStaticFile("/config.js", "application/javascript; charset=utf-8");
  });
  server.on("/mock.js", HTTP_GET, []() {
    serveStaticFile("/mock.js", "application/javascript; charset=utf-8");
  });
  server.on("/state_machine.js", HTTP_GET, []() {
    serveStaticFile("/state_machine.js", "application/javascript; charset=utf-8");
  });
  server.on("/api.js", HTTP_GET, []() {
    serveStaticFile("/api.js", "application/javascript; charset=utf-8");
  });
  server.on("/app.js", HTTP_GET, []() {
    serveStaticFile("/app.js", "application/javascript; charset=utf-8");
  });

  server.onNotFound([]() {
    server.send(404, "text/plain; charset=utf-8", "Ruta no encontrada");
  });
}

} // namespace

void startWaterAlphaServer() {
  WiFi.mode(WIFI_AP);
  const bool apStarted = WiFi.softAP(
      WAConfig::WIFI_SSID,
      WAConfig::WIFI_PASSWORD
  );
  Serial.println(apStarted
      ? "[WEB] SoftAP iniciado" : "[WEB] ERROR al iniciar SoftAP");
  Serial.print("[WEB] IP local: ");
  Serial.println(WiFi.softAPIP());

  // false evita formatear/borrar la particion automaticamente si falla.
  filesystemReady = LittleFS.begin(false);
  Serial.println(filesystemReady
      ? "[WEB] LittleFS montado" : "[WEB] LittleFS no disponible");

  registerRoutes();
  server.begin();
  Serial.println("[WEB] Servidor HTTP iniciado");
}

void updateWaterAlphaServer() {
  server.handleClient();
}
