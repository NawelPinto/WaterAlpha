
#include "mock_sensors.h"

namespace WAMock {

  WA::Readings read(uint32_t elapsedMs) {

    // Condiciones iniciales simuladas
    WA::Readings r = {
      7.2f,   // pH
      17.8f,  // Temperatura
      true,   // Sensor pH OK
      true,   // Sensor temperatura OK
      true,   // Lectura estable
      true,   // Calibración válida
      false   // Inicializando
    };

    // Primeros tres segundos: inicialización
    if (elapsedMs < 3000) {
      r.initializing = true;
      return r;
    }

    // Cambiar escenario cada cinco segundos
    uint32_t elapsed = elapsedMs - 3000;
    uint32_t scenario = (elapsed / 5000) % 12;

    switch (scenario) {

      case 0:
        r.ph = 7.2f;
        break;

      case 1:
        r.ph = 8.45f;
        break;

      case 2:
        r.ph = 8.55f;
        break;

      case 3:
        r.ph = 8.45f;
        break;

      case 4:
        r.ph = 8.4f;
        break;

      case 5:
        r.ph = 7.2f;
        break;

      case 6:
        r.ph = 5.8f;
        break;

      case 7:
        r.ph = 6.55f;
        break;

      case 8:
        r.ph = 6.6f;
        break;

      case 9:
        r.ph = 7.2f;
        break;

      case 10:
        r.ph_sensor_ok = false;
        break;

      case 11:
        r.stable = false;
        break;
    }

    return r;
  }

}
