
#include "led_colors.h"

namespace WAColors {

  Channels fromState(WA::State state) {

    switch (state) {

      case WA::State::BLUE:
        return {false, false, true};

      case WA::State::GREEN:
        return {false, true, false};

      case WA::State::YELLOW:
        return {true, true, false};

      case WA::State::RED:
        return {true, false, false};

      case WA::State::MAGENTA:
        return {true, false, true};

      default:
        // Error desconocido: magenta.
        return {true, false, true};
    }
  }

}
