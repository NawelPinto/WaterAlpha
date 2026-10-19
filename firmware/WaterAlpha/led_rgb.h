
#ifndef WATERALPHA_LED_RGB_H
#define WATERALPHA_LED_RGB_H

#include "types.h"

namespace WALed {

  // Preparar controlador fisico
  void begin();

  // Mostrar un estado del sistema
  void setState(WA::State state);

}

#endif
