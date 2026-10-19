
#include <cmath>
#include <cstdint>
#include <iostream>

#include "../firmware/WaterAlpha/mock_sensors.h"
#include "../firmware/WaterAlpha/state_machine.h"
#include "../firmware/WaterAlpha/ph_calibration.h"
#include "../firmware/WaterAlpha/ph_pipeline.h"

struct TestCase {
  uint32_t timeMs;
  WA::State expectedState;
  bool expectedReadingValid;
};

int main() {

  WA::StateMachine machine;

  // Secuencia cronologica del simulador.
  // Todos los tiempos son desde el arranque.
  const TestCase cases[] = {
    {    0, WA::State::BLUE,    false },
    { 3000, WA::State::GREEN,   true  },
    { 8000, WA::State::YELLOW,  true  },
    {13000, WA::State::RED,     true  },
    {18000, WA::State::RED,     true  },
    {23000, WA::State::YELLOW,  true  },
    {28000, WA::State::GREEN,   true  },
    {33000, WA::State::RED,     true  },
    {38000, WA::State::RED,     true  },
    {43000, WA::State::YELLOW,  true  },
    {48000, WA::State::GREEN,   true  },
    {53000, WA::State::MAGENTA, false },
    {58000, WA::State::YELLOW,  false },
    {63000, WA::State::GREEN,   true  }
  };

  int passed = 0;

  std::cout
    << "=== WATERALPHA: FLUJO SIMULADO ===\n";

  for (const TestCase& test : cases) {

    // Obtener lectura ficticia del instante.
    WA::Readings readings =
      WAMock::read(test.timeMs);

    // Evaluar manteniendo memoria de histéresis.
    WA::Decision result =
      machine.evaluate(readings);

    bool correct =
      result.state == test.expectedState &&
      result.reading_valid ==
        test.expectedReadingValid;

    std::cout
      << "t=" << test.timeMs << " ms"
      << " | estado=" << WA::stateName(result.state)
      << " | " << (correct ? "OK" : "ERROR")
      << "\n";

    if (!correct) {
      std::cerr
        << "Se esperaba: "
        << WA::stateName(test.expectedState)
        << "\n";

      return 1;
    }

    passed++;
  }

  // PRUEBA DE LA CADENA DE CALIBRACION
  // Solo utilizamos datos sinteticos.

  const WAPhCalibration::Point p1{
    1200.0, 4.0
  };

  const WAPhCalibration::Point p2{
    1500.0, 7.0
  };

  auto model =
    WAPhCalibration::fitTwoPoint(p1, p2);

  if (!model.valid) {
    std::cerr << "Error al crear calibracion\n";
    return 1;
  }

  // Sin evidencia real, debe rechazarla.
  WAPhPipeline::Evidence unverified{
    false,
    false
  };

  auto rejected =
    WAPhPipeline::process(
      1350.0,
      true,
      model,
      unverified
    );

  if (
    rejected.ph_sensor_ok ||
    rejected.calibrated ||
    !std::isnan(rejected.ph)
  ) {
    std::cerr
      << "ERROR: se acepto lectura no validada\n";
    return 1;
  }

  passed++;

  // Simulamos evidencia positiva solamente
  // dentro de esta prueba automatica.
  WAPhPipeline::Evidence testEvidence{
    true,
    true
  };

  auto estimated =
    WAPhPipeline::process(
      1350.0,
      true,
      model,
      testEvidence
    );

  if (
    !estimated.ph_sensor_ok ||
    !estimated.calibrated ||
    std::fabs(estimated.ph - 5.5f) > 0.001f
  ) {
    std::cerr
      << "ERROR: conversion de pH inesperada\n";
    return 1;
  }

  passed++;

  // Entregar el resultado a la maquina
  // de estados como prueba de integracion.
  WA::Readings integrated =
    WAMock::read(3000);

  integrated.ph = estimated.ph;
  integrated.ph_sensor_ok =
    estimated.ph_sensor_ok;
  integrated.calibrated =
    estimated.calibrated;

  WA::StateMachine integrationMachine;

  auto finalDecision =
    integrationMachine.evaluate(integrated);

  if (
    finalDecision.state != WA::State::RED ||
    !finalDecision.reading_valid
  ) {
    std::cerr
      << "ERROR: integracion pH-estado\n";
    return 1;
  }

  passed++;

  std::cout << "\n";
  std::cout
    << "WaterAlpha: "
    << passed
    << " comprobaciones integradas OK\n";

  return 0;
}
