
#include <cassert>
#include <iostream>
#include <limits>

#include "../firmware/WaterAlpha/state_machine.h"

using namespace WA;

int main() {

  StateMachine machine;

  Readings r = {
    7.2f,   // pH
    17.8f,  // temperatura
    true,   // sensor pH
    true,   // sensor temperatura
    true,   // estable
    true,   // calibrado
    false   // inicializando
  };

  // Lectura normal
  assert(machine.evaluate(r).state == State::GREEN);

  // Precaucion
  r.ph = 8.45f;
  assert(machine.evaluate(r).state == State::YELLOW);

  // Anomalia superior
  r.ph = 8.55f;
  assert(machine.evaluate(r).state == State::RED);

  // Histeresis: permanece rojo
  r.ph = 8.45f;
  assert(machine.evaluate(r).state == State::RED);

  // Sale de rojo al llegar a 8.4
  r.ph = 8.4f;
  assert(machine.evaluate(r).state == State::YELLOW);

  // Regresa a verde
  r.ph = 7.2f;
  assert(machine.evaluate(r).state == State::GREEN);

  // Anomalia inferior
  r.ph = 5.8f;
  assert(machine.evaluate(r).state == State::RED);

  // Sensor desconectado
  r.ph_sensor_ok = false;
  assert(machine.evaluate(r).state == State::MAGENTA);

  // Inicializacion
  r.ph_sensor_ok = true;
  r.ph = 7.2f;
  r.initializing = true;
  assert(machine.evaluate(r).state == State::BLUE);

  // Lectura inestable
  r.initializing = false;
  r.stable = false;
  assert(machine.evaluate(r).state == State::YELLOW);

  // Lectura imposible
  r.stable = true;
  r.ph = std::numeric_limits<float>::quiet_NaN();
  assert(machine.evaluate(r).state == State::MAGENTA);

  std::cout << "WaterAlpha: pruebas OK" << std::endl;

  return 0;
}
