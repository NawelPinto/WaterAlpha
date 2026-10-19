
#ifndef WATERALPHA_CONFIG_H
#define WATERALPHA_CONFIG_H

#include <Arduino.h>

// WATERALPHA - CONFIGURACIÓN ALPHA

namespace WAConfig {

  // Nombre de la red creada por el ESP32
  constexpr char WIFI_SSID[] = "WaterAlpha";

  // Clave provisional de desarrollo
  constexpr char WIFI_PASSWORD[] = "WaterAlphaTest26";

  // Puerto del servidor HTTP
  constexpr uint16_t HTTP_PORT = 80;

  // Velocidad del monitor serial
  constexpr unsigned long SERIAL_BAUD = 115200;

  // Referencias preliminares de pH
  constexpr float PH_MIN = 6.5f;
  constexpr float PH_MAX = 8.5f;

  // Parámetros de diseño, no normativos
  constexpr float PH_CAUTION_MARGIN = 0.1f;
  constexpr float PH_HYSTERESIS = 0.1f;

  // El firmware aún utilizará valores simulados
  constexpr bool MOCK_MODE = true;
}

#endif
