#ifndef WATERALPHA_LED_STATUS_LOGIC_H
#define WATERALPHA_LED_STATUS_LOGIC_H
#include <stdint.h>
#include "types.h"
// Tres indicadores discretos; blue=amarillo parpadeante, magenta=rojo parpadeante.
// No interpretar el verde como potabilidad.
namespace WALedLogic {
 struct Output{bool red;bool green;bool yellow;};
 inline Output fromState(WA::State s,uint32_t ms) {
  switch(s) {
   case WA::State::GREEN: return {false,true,false};
   case WA::State::YELLOW: return {false,false,true};
   case WA::State::RED: return {true,false,false};
   case WA::State::BLUE: return {false,false,(ms/500u)%2u==0u};
   case WA::State::MAGENTA: return {(ms/250u)%2u==0u,false,false};
   default:return {true,false,false};
  }
 }
}
#endif
