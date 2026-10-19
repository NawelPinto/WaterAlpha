
#ifndef WATERALPHA_PH_SENSOR_H
#define WATERALPHA_PH_SENSOR_H

#include <stdint.h>

namespace WAPh {

  // Resultado de la adquisicion analogica
  struct RawReading {

    uint32_t adcMillivolts;

    // Indica que se intento adquirir
    // una muestra desde el ADC.
    // NO confirma que el electrodo
    // este conectado o funcionando.
    bool acquired;
  };

  // Preparar el canal de adquisicion
  void begin();

  // Leer el voltaje en la entrada ADC
  RawReading read();

}

#endif
