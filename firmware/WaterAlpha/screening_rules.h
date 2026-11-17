#ifndef WATERALPHA_SCREENING_RULES_H
#define WATERALPHA_SCREENING_RULES_H

// WaterAlpha - tamizaje experimental. NO determina potabilidad.
// Referencia experimental de pH del equipo: 6.5 < pH < 8.5; no certifica NCh409.
// Margen precautorio: 0.32 pH (discrepancia observada en el ensayo
// intermedio; NO es incertidumbre metrologica certificada).
// Temperatura: 20.9 +/- 2 C (ventana operativa de diseno,
// NO limite sanitario ni compensacion termica).

#include <cmath>
#include <stdint.h>

namespace WAScreening {

constexpr double PH_MIN = 6.5;
constexpr double PH_MAX = 8.5;
constexpr double PH_CAUTION = 0.32;
constexpr double CAL_T_C = 20.9;
constexpr double CAL_T_TOLERANCE_C = 2.0;
constexpr uint32_t MAX_AGE_MS = 4000;

struct Result {
  const char *status;     // NO_EVALUABLE, ALERTA_PH, PRECAUCION, PH_DENTRO_REFERENCIA
  const char *message;    // Diagnostico preliminar, nunca potabilidad
  const char *phBand;     // SIN_DATOS, FUERA, CERCA_LIMITE, DENTRO
  const char *tempBand;   // SIN_DATOS, CERCANA_CALIBRACION, FUERA_VENTANA_EXPERIMENTAL
};

inline Result evaluate(bool realMode, bool temperatureSensorOk,
                       double temperatureC, double estimatedPh,
                       uint32_t ageMs) {
  if (!realMode) {
    return {"NO_EVALUABLE", "Modo simulado: sin evaluacion fisica.",
            "SIN_DATOS", "SIN_DATOS"};
  }

  const bool tempOk = temperatureSensorOk && std::isfinite(temperatureC);
  const bool phOk = std::isfinite(estimatedPh);
  const char *tBand = tempOk
      ? ((std::fabs(temperatureC - CAL_T_C) <= CAL_T_TOLERANCE_C)
          ? "CERCANA_CALIBRACION" : "FUERA_VENTANA_EXPERIMENTAL")
      : "SIN_DATOS";

  if (!tempOk || !phOk || ageMs > MAX_AGE_MS) {
    return {"NO_EVALUABLE",
            "Sin lectura suficiente, actualizada o dentro del rango calibrado.",
            "SIN_DATOS", tBand};
  }

  // Los limites de la norma son estrictos; los extremos no se consideran dentro.
  const bool outside = (estimatedPh <= PH_MIN || estimatedPh >= PH_MAX);
  const bool near = (estimatedPh < PH_MIN + PH_CAUTION ||
                     estimatedPh > PH_MAX - PH_CAUTION);

  if (outside) {
    return {"ALERTA_PH",
            "Indicio de pH estimado fuera de referencia. Requiere confirmacion.",
            "FUERA", tBand};
  }

  if (near) {
    return {"PRECAUCION",
            "pH estimado cercano a un limite; resultado no concluyente.",
            "CERCA_LIMITE", tBand};
  }

  if (std::fabs(temperatureC - CAL_T_C) > CAL_T_TOLERANCE_C) {
    return {"PRECAUCION",
            "pH estimado dentro de referencia, pero fuera de la ventana termica ensayada.",
            "DENTRO", tBand};
  }

  return {"PH_DENTRO_REFERENCIA",
          "Estimacion de pH dentro de referencia experimental; no determina potabilidad.",
          "DENTRO", tBand};
}

} // namespace WAScreening

#endif
