
#ifndef WATERALPHA_PH_PIPELINE_H
#define WATERALPHA_PH_PIPELINE_H

#include "ph_calibration.h"

namespace WAPhPipeline {

  // Evidencias requeridas para aceptar
  // una estimacion de pH.
  struct Evidence {

    // No basta con que el ADC entregue
    // un numero: la senal debe validarse.
    bool signal_validated;

    // Calibracion fisica comprobada.
    bool calibration_verified;
  };

  // Resultado del procesamiento.
  struct Reading {

    float ph;

    bool ph_sensor_ok;

    bool calibrated;
  };

  // Convertir una lectura ADC a pH
  // solamente si supera las validaciones.
  Reading process(
    double adcMillivolts,
    bool acquired,
    const WAPhCalibration::Calibration& model,
    const Evidence& evidence
  );

}

#endif
