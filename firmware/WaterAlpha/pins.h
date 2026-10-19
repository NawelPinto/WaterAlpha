
#ifndef WATERALPHA_PINS_H
#define WATERALPHA_PINS_H

/*
 * WATERALPHA - MAPA DE PINES
 *
 * Placa: UAI personalizada con ESP-32S
 *
 * IMPORTANTE:
 * Todos los pines quedan sin asignar
 * hasta verificar el pinout real.
 */

namespace WAPins {

  // Sensor digital DS18B20
  constexpr int PIN_TEMP = -1;

  // Entrada analógica: usar ADC1
  constexpr int PIN_PH_ADC = -1;

  // LED RGB
  constexpr int PIN_LED_R = -1;
  constexpr int PIN_LED_G = -1;
  constexpr int PIN_LED_B = -1;

  // Seguridad: no activar hardware todavía
  constexpr bool PINOUT_CONFIRMED = false;

}

#endif
