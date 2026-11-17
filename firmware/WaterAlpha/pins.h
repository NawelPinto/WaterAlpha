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
  constexpr int PIN_TEMP = -1; // Pendiente verificar exposicion de GPIO25 en placa UAI

  // Entrada analógica: usar ADC1
  constexpr int PIN_PH_ADC = -1; // Pendiente verificar exposicion de GPIO34 en placa UAI

  // LED RGB
  constexpr int PIN_LED_R = -1;
  constexpr int PIN_LED_G = -1;
  constexpr int PIN_LED_B = -1; // Nombre historico: canal AMARILLO en 3 LEDs discretos

  
  // Tipo de LED por confirmar.
  constexpr bool LED_TYPE_CONFIRMED = false;

  // Valor provisional, NO habilitar hardware
  // hasta conocer el tipo de LED.
  // true: anodo comun
  // false: catodo comun
  constexpr bool LED_COMMON_ANODE = false;


  // Seguridad: no activar hardware todavía
  constexpr bool PINOUT_CONFIRMED = false; // Bloqueo preventivo

}

#endif