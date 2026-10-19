
#include "ph_pipeline.h"

#include <limits>

namespace WAPhPipeline {

  Reading invalidReading() {

    return {
      std::numeric_limits<float>::quiet_NaN(),
      false,
      false
    };
  }

  Reading process(
    double adcMillivolts,
    bool acquired,
    const WAPhCalibration::Calibration& model,
    const Evidence& evidence
  ) {

    // 1. Verificar que se adquirio una muestra.
    if (!acquired) {
      return invalidReading();
    }

    // 2. Verificar integridad de la senal.
    if (!evidence.signal_validated) {
      return invalidReading();
    }

    // 3. Exigir calibracion fisica verificada.
    if (
      !evidence.calibration_verified ||
      !model.valid
    ) {
      return invalidReading();
    }

    // 4. Convertir voltaje a pH.
    WAPhCalibration::Result estimated =
      WAPhCalibration::estimatePh(
        model,
        adcMillivolts
      );

    // 5. Rechazar conversiones invalidas.
    if (!estimated.valid) {
      return invalidReading();
    }

    // 6. Aceptar la medicion.
    return {
      static_cast<float>(estimated.ph),
      true,
      true
    };
  }

}
