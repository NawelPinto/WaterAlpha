#ifndef WATERALPHA_LED_STATUS_LOGIC_H
#define WATERALPHA_LED_STATUS_LOGIC_H
#include <stdint.h>
#include "types.h"
namespace WALedLogic {
 struct Output { bool red; bool green; bool yellow; };
 // BLUE: amarillo lento; MAGENTA: rojo rapido. Ningun color representa potabilidad.
 inline Output fromState(WA::State state, uint32_t millisNow) {
  switch(state) {
   case WA::State::GREEN: return {false,true,false};
   case WA::State::YELLOW: return {false,false,true};
   case WA::State::RED: return {true,false,false};
   case WA::State::BLUE: return {false,false,(millisNow/500u)%2u==0u};
   case WA::State::MAGENTA: return {(millisNow/250u)%2u==0u,false,false};
   default: return {true,false,false};
  }
 }
}
#endif
