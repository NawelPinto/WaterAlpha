
#ifndef WATERALPHA_MOCK_SENSORS_H
#define WATERALPHA_MOCK_SENSORS_H

#include <stdint.h>
#include "types.h"

namespace WAMock {

  // Genera lecturas a partir del tiempo transcurrido
  WA::Readings read(uint32_t elapsedMs);

}

#endif
