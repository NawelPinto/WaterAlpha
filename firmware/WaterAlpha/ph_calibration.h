
#ifndef WATERALPHA_PH_CALIBRATION_H
#define WATERALPHA_PH_CALIBRATION_H

namespace WAPhCalibration {

  // Punto experimental de calibracion
  struct Point {
    double millivolts;
    double ph;
  };

  // Modelo matematico
  struct Calibration {
    double slope;
    double offset;
    bool valid;
  };

  // Resultado de convertir voltaje a pH
  struct Result {
    double ph;
    bool valid;
  };

  // Calcular pendiente e intercepto
  Calibration fitTwoPoint(
    const Point& point1,
    const Point& point2
  );

  // Aplicar modelo a una lectura
  Result estimatePh(
    const Calibration& calibration,
    double millivolts
  );

}

#endif
