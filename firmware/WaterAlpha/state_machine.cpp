
#include "state_machine.h"
#include "config.h"

#include <cmath>

namespace WA {

  // Convertir estados a texto
  const char* stateName(State state) {

    switch (state) {

      case State::BLUE:    return "BLUE";
      case State::GREEN:   return "GREEN";
      case State::YELLOW:  return "YELLOW";
      case State::RED:     return "RED";
      case State::MAGENTA: return "MAGENTA";

      default: return "MAGENTA";
    }
  }

  // Restablecer memoria
  void StateMachine::reset() {
    anomaly_ = Anomaly::NONE;
  }

  // Evaluar mediciones
  Decision StateMachine::evaluate(
    const Readings& r
  ) {

    // 1. Comprobar fallos conocidos
    if (
      !r.ph_sensor_ok ||
      !r.temperature_sensor_ok ||
      !r.calibrated
    ) {

      reset();

      return {
        State::MAGENTA,
        "Sensor o calibracion invalida.",
        false
      };
    }

    // 2. Inicializacion
    if (r.initializing) {

      reset();

      return {
        State::BLUE,
        "Esperando mediciones validas.",
        false
      };
    }

    // 3. Validar mediciones numericas
    bool phValid =
      std::isfinite(r.ph) &&
      r.ph >= WAConfig::PH_PHYSICAL_MIN &&
      r.ph <= WAConfig::PH_PHYSICAL_MAX;

    bool tempValid =
      std::isfinite(r.temperature) &&
      r.temperature >= WAConfig::TEMP_SENSOR_MIN &&
      r.temperature <= WAConfig::TEMP_SENSOR_MAX;

    if (!phValid || !tempValid) {

      reset();

      return {
        State::MAGENTA,
        "Lectura numerica invalida.",
        false
      };
    }

    // 4. Estabilidad
    if (!r.stable) {

      return {
        State::YELLOW,
        "Lectura inestable.",
        false
      };
    }

    // 5. Detectar anomalías
    if (r.ph > WAConfig::PH_MAX) {
      anomaly_ = Anomaly::ABOVE_MAX;
    }
    else if (r.ph < WAConfig::PH_MIN) {
      anomaly_ = Anomaly::BELOW_MIN;
    }

    // 6. Histéresis superior
    else if (
      anomaly_ == Anomaly::ABOVE_MAX &&
      r.ph <= (
        WAConfig::PH_MAX -
        WAConfig::PH_HYSTERESIS
      )
    ) {
      anomaly_ = Anomaly::NONE;
    }

    // 7. Histéresis inferior
    else if (
      anomaly_ == Anomaly::BELOW_MIN &&
      r.ph >= (
        WAConfig::PH_MIN +
        WAConfig::PH_HYSTERESIS
      )
    ) {
      anomaly_ = Anomaly::NONE;
    }

    // 8. Mantener anomalía activa
    if (anomaly_ != Anomaly::NONE) {

      return {
        State::RED,
        "pH fuera de referencia.",
        true
      };
    }

    // 9. Zona amarilla
    bool nearLower =
      r.ph <= (
        WAConfig::PH_MIN +
        WAConfig::PH_CAUTION_MARGIN
      );

    bool nearUpper =
      r.ph >= (
        WAConfig::PH_MAX -
        WAConfig::PH_CAUTION_MARGIN
      );

    if (nearLower || nearUpper) {

      return {
        State::YELLOW,
        "pH cercano a un limite.",
        true
      };
    }

    // 10. Sin anomalía detectada
    return {
      State::GREEN,
      "pH dentro de referencia.",
      true
    };
  }

}
