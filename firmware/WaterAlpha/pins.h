#ifndef WATERALPHA_PINS_H
#define WATERALPHA_PINS_H
/*
 * Placa UAI Ai-Thinker ESP-32S. Los GPIO de sensores provienen del
 * snapshot local anterior: CONFIRMAR que son los usados en la placa actual.
 * NO modificar un sensor que ya fue conectado/probado sin verificar.
 */
namespace WAPins {
  constexpr int PIN_TEMP = 25;      // DS18B20: verificar GPIO real
  constexpr int PIN_PH_ADC = 34;    // ADC1: verificar GPIO real
  constexpr bool PINOUT_CONFIRMED = true; // Solo dejar true si ambos GPIO se verificaron fisicamente

  // TRES LED discretos (cada uno con su propia resistencia serie).
  // NO son canales de un LED RGB. Rellenar tras revisar disponibilidad UAI.
  constexpr int PIN_LED_RED = -1;
  constexpr int PIN_LED_GREEN = -1;
  constexpr int PIN_LED_YELLOW = -1;
  constexpr bool LED_PINS_CONFIRMED = false;
  constexpr bool LED_CURRENT_LIMITING_CONFIRMED = false;
  constexpr bool LED_POLARITY_CONFIRMED = false;
}
#endif
