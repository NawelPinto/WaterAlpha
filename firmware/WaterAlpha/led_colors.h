
#ifndef WATERALPHA_LED_COLORS_H
#define WATERALPHA_LED_COLORS_H

#include "types.h"

namespace WAColors {

  struct Channels {
    bool red;
    bool green;
    bool blue;
  };

  // Convertir un estado a canales RGB.
  Channels fromState(WA::State state);

}

#endif
