
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

#include "../firmware/WaterAlpha/ph_calibration.h"

using namespace WAPhCalibration;

// Comparar decimales con tolerancia
bool approximatelyEqual(
  double a,
  double b
) {
  return std::fabs(a - b) < 0.000001;
}

int main() {

  // DATOS SINTETICOS, NO REALES
  Point p1 = {1200.0, 4.0};
  Point p2 = {1500.0, 7.0};

  // Prueba 1: construir calibracion
  Calibration model = fitTwoPoint(p1, p2);

  assert(model.valid);

  // Prueba 2: pendiente
  assert(
    approximatelyEqual(model.slope, 0.01)
  );

  // Prueba 3: intercepto
  assert(
    approximatelyEqual(model.offset, -8.0)
  );

  // Prueba 4: recuperar primer patron
  Result r1 = estimatePh(model, 1200.0);

  assert(r1.valid);
  assert(approximatelyEqual(r1.ph, 4.0));

  // Prueba 5: recuperar segundo patron
  Result r2 = estimatePh(model, 1500.0);

  assert(r2.valid);
  assert(approximatelyEqual(r2.ph, 7.0));

  // Prueba 6: interpolacion
  Result r3 = estimatePh(model, 1350.0);

  assert(r3.valid);
  assert(approximatelyEqual(r3.ph, 5.5));

  // Prueba 7: voltajes identicos
  Point badPoint = {1200.0, 7.0};

  Calibration badModel =
    fitTwoPoint(p1, badPoint);

  assert(!badModel.valid);

  // Prueba 8: impedir estimar sin modelo
  Result invalid = estimatePh(
    badModel,
    1350.0
  );

  assert(!invalid.valid);

  // Prueba 9: voltaje negativo
  assert(
    !estimatePh(model, -5.0).valid
  );

  // Prueba 10: lectura NaN
  double nanValue =
    std::numeric_limits<double>::quiet_NaN();

  assert(
    !estimatePh(model, nanValue).valid
  );

  // Prueba 11: pH fuera del rango permitido
  assert(
    !estimatePh(model, 2500.0).valid
  );

  // Prueba 12: pendiente negativa
  Point reverse1 = {1200.0, 7.0};
  Point reverse2 = {1500.0, 4.0};

  Calibration reverseModel =
    fitTwoPoint(reverse1, reverse2);

  assert(reverseModel.valid);

  Result reverseResult =
    estimatePh(reverseModel, 1350.0);

  assert(reverseResult.valid);
  assert(
    approximatelyEqual(reverseResult.ph, 5.5)
  );

  std::cout
    << "WaterAlpha: 12 pruebas de calibracion OK"
    << std::endl;

  return 0;
}
