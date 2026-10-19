
#ifndef WATERALPHA_CONFIG_H
#define WATERALPHA_CONFIG_H

#include <stdint.h>

// WATERALPHA - CONFIGURACIÓN ALPHA

namespace WAConfig {

  
  // Fuente de mediciones de pH
  // false: simulacion
  // true: adquirir desde el ADC1
  constexpr bool USE_REAL_PH = false;

  // Fuente de temperatura
  // false: simulada
  // true: DS18B20 fisico
  constexpr bool USE_REAL_TEMPERATURE = false;

  // Tiempo maximo de conversion a 12 bits
  constexpr uint32_t TEMP_CONVERSION_MS = 750;


  // Periodo de actualización del sistema
  constexpr uint32_t SAMPLE_PERIOD_MS = 1000;
  
  // Límites para detectar lecturas imposibles
  constexpr float PH_PHYSICAL_MIN = 0.0f;
  constexpr float PH_PHYSICAL_MAX = 14.0f;

  // Rango nominal del DS18B20
  constexpr float TEMP_SENSOR_MIN = -55.0f;
  constexpr float TEMP_SENSOR_MAX = 125.0f;

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
