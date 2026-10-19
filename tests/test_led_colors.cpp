
#include <cassert>
#include <iostream>

#include "../firmware/WaterAlpha/led_colors.h"

using namespace WA;
using namespace WAColors;

int main() {

  Channels blue = fromState(State::BLUE);

  assert(
    !blue.red &&
    !blue.green &&
    blue.blue
  );

  Channels green = fromState(State::GREEN);

  assert(
    !green.red &&
    green.green &&
    !green.blue
  );

  Channels yellow = fromState(State::YELLOW);

  assert(
    yellow.red &&
    yellow.green &&
    !yellow.blue
  );

  Channels red = fromState(State::RED);

  assert(
    red.red &&
    !red.green &&
    !red.blue
  );

  Channels magenta = fromState(State::MAGENTA);

  assert(
    magenta.red &&
    !magenta.green &&
    magenta.blue
  );

  std::cout
    << "WaterAlpha: 5 pruebas LED OK"
    << std::endl;

  return 0;
}
