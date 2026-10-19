
#ifndef WATERALPHA_TYPES_H
#define WATERALPHA_TYPES_H

#include <stdint.h>

namespace WA {

  // Estados posibles
  enum class State : uint8_t {
    BLUE,
    GREEN,
    YELLOW,
    RED,
    MAGENTA
  };

  // Datos recibidos por la máquina
  struct Readings {

    float ph;
    float temperature;

    bool ph_sensor_ok;
    bool temperature_sensor_ok;

    bool stable;
    bool calibrated;
    bool initializing;
  };

  // Resultado del procesamiento
  struct Decision {

    State state;

    const char* message;

    bool reading_valid;
  };

  // Convertir estado a texto
  const char* stateName(State state);

}

#endif
