
#ifndef WATERALPHA_TEMPERATURE_SENSOR_H
#define WATERALPHA_TEMPERATURE_SENSOR_H

namespace WATemperature {

  // Resultado de una medicion
  struct Reading {
    float celsius;
    bool sensor_ok;
  };

  // Preparar el sensor
  void begin();

  // Obtener la ultima medicion disponible
  Reading read();

}

#endif
