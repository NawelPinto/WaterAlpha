
#ifndef WATERALPHA_CONTROLLER_H
#define WATERALPHA_CONTROLLER_H

#include <stdint.h>
#include "types.h"

namespace WAController {

  struct Snapshot {

    WA::Readings readings;
    WA::Decision decision;

    uint32_t sample_age_ms;
  };

  // Inicializar controlador
  void begin();

  // Actualizar adquisición y estado
  void update();

  // Obtener última medición procesada
  Snapshot snapshot();

}

#endif
