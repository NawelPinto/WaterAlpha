
#include <cassert>
#include <cmath>
#include <iostream>

#include "../firmware/WaterAlpha/ph_pipeline.h"

using namespace WAPhCalibration;
using namespace WAPhPipeline;

int main() {

  // Datos exclusivamente sinteticos.
  Point p1 = {1200.0, 4.0};
  Point p2 = {1500.0, 7.0};

  Calibration model = fitTwoPoint(p1, p2);

  assert(model.valid);

  // Caso 1: ninguna evidencia fisica.
  Evidence unverified = {false, false};

  Reading r1 = process(
    1350.0,
    true,
    model,
    unverified
  );

  assert(!r1.ph_sensor_ok);
  assert(!r1.calibrated);
  assert(std::isnan(r1.ph));

  // Caso 2: ADC sin adquisicion.
  Evidence verified = {true, true};

  Reading r2 = process(
    1350.0,
    false,
    model,
    verified
  );

  assert(!r2.ph_sensor_ok);

  // Caso 3: modelo no verificado.
  Evidence noCalibration = {true, false};

  Reading r3 = process(
    1350.0,
    true,
    model,
    noCalibration
  );

  assert(!r3.calibrated);

  // Caso 4: condiciones sinteticas validas.
  Reading r4 = process(
    1350.0,
    true,
    model,
    verified
  );

  assert(r4.ph_sensor_ok);
  assert(r4.calibrated);
  assert(std::fabs(r4.ph - 5.5f) < 0.001f);

  // Caso 5: conversion fuera de rango.
  Reading r5 = process(
    2500.0,
    true,
    model,
    verified
  );

  assert(!r5.ph_sensor_ok);
  assert(std::isnan(r5.ph));

  std::cout
    << "WaterAlpha: 5 pruebas de pipeline OK"
    << std::endl;

  return 0;
}
