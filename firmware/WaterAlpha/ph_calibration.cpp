
#include "ph_calibration.h"
#include "config.h"

#include <cmath>
#include <limits>

namespace WAPhCalibration {

  // Resultado invalido por defecto
  Result invalidResult() {
    return {
      std::numeric_limits<double>::quiet_NaN(),
      false
    };
  }

  Calibration fitTwoPoint(
    const Point& p1,
    const Point& p2
  ) {

    Calibration invalid = {0.0, 0.0, false};

    // Comprobar numeros finitos
    if (
      !std::isfinite(p1.millivolts) ||
      !std::isfinite(p2.millivolts) ||
      !std::isfinite(p1.ph) ||
      !std::isfinite(p2.ph)
    ) {
      return invalid;
    }

    // Los voltajes no pueden ser negativos
    if (
      p1.millivolts < 0.0 ||
      p2.millivolts < 0.0
    ) {
      return invalid;
    }

    // Comprobar rango fisico de pH
    if (
      p1.ph < WAConfig::PH_PHYSICAL_MIN ||
      p1.ph > WAConfig::PH_PHYSICAL_MAX ||
      p2.ph < WAConfig::PH_PHYSICAL_MIN ||
      p2.ph > WAConfig::PH_PHYSICAL_MAX
    ) {
      return invalid;
    }

    // Evitar dos patrones de igual pH
    if (p1.ph == p2.ph) {
      return invalid;
    }

    double deltaV =
      p2.millivolts - p1.millivolts;

    // Evitar division por cero y una
    // diferencia de voltaje casi nula.
    if (std::fabs(deltaV) < 1.0) {
      return invalid;
    }

    // Calcular pendiente
    double slope =
      (p2.ph - p1.ph) / deltaV;

    // Calcular intercepto
    double offset =
      p1.ph - slope * p1.millivolts;

    if (
      !std::isfinite(slope) ||
      !std::isfinite(offset)
    ) {
      return invalid;
    }

    return {
      slope,
      offset,
      true
    };
  }


  Result estimatePh(
    const Calibration& calibration,
    double millivolts
  ) {

    // Comprobar que el modelo existe
    if (
      !calibration.valid ||
      !std::isfinite(calibration.slope) ||
      !std::isfinite(calibration.offset)
    ) {
      return invalidResult();
    }

    // Validar lectura de voltaje
    if (
      !std::isfinite(millivolts) ||
      millivolts < 0.0
    ) {
      return invalidResult();
    }

    // Aplicar ecuacion lineal
    double ph =
      calibration.slope * millivolts +
      calibration.offset;

    // Rechazar valores fuera de 0-14
    if (
      !std::isfinite(ph) ||
      ph < WAConfig::PH_PHYSICAL_MIN ||
      ph > WAConfig::PH_PHYSICAL_MAX
    ) {
      return invalidResult();
    }

    return {
      ph,
      true
    };
  }

}
