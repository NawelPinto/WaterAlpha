#ifndef WATERALPHA_LED_RGB_H
#define WATERALPHA_LED_RGB_H
#include "types.h"
// Nombre heredado para compatibilidad; maneja tres LEDs independientes.
namespace WALed {
 void begin();
 void setState(WA::State state);
 void update(); // Parpadeo no bloqueante, llamar en loop()
}
#endif
