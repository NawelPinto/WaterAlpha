#ifndef WATERALPHA_LED_RGB_H
#define WATERALPHA_LED_RGB_H
#include "types.h"
// Nombre heredado; maneja tres LED discretos rojo, verde y amarillo.
namespace WALed {
 void begin();
 void setState(WA::State state);
 void update();
}
#endif
