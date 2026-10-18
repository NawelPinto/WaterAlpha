
/*
 * WATERALPHA - MÁQUINA DE ESTADOS
 * Bloque 3
 */

window.WaterAlphaStateMachine = (() => {

  const config = window.WaterAlphaConfig;

  // Memoria de anomalías anteriores
  let anomaly = null;

  function reset() {
    anomaly = null;
  }

  function result(state, title, message) {
    return { state, title, message };
  }

  function evaluate(data) {

    // 1. Fallos técnicos conocidos
    if (
      !data ||
      !data.ph_sensor_ok ||
      !data.temperature_sensor_ok ||
      !data.calibrated
    ) {
      reset();

      return result(
        "MAGENTA",
        "Error del sistema",
        "Sensor desconectado o calibración inválida."
      );
    }

    // 2. Inicialización
    if (data.initializing) {
      reset();

      return result(
        "BLUE",
        "Inicializando",
        "Esperando mediciones válidas."
      );
    }

    // 3. Validar valores numéricos
    const validPH =
      Number.isFinite(data.ph) &&
      data.ph >= config.PH_PHYSICAL_MIN &&
      data.ph <= config.PH_PHYSICAL_MAX;

    const validTemp =
      Number.isFinite(data.temperature) &&
      data.temperature >= config.TEMP_SENSOR_MIN &&
      data.temperature <= config.TEMP_SENSOR_MAX;

    if (!validPH || !validTemp) {
      reset();

      return result(
        "MAGENTA",
        "Lectura inválida",
        "Se detectaron valores imposibles o ausentes."
      );
    }

    // 4. Lectura inestable
    if (!data.stable) {
      return result(
        "YELLOW",
        "Precaución",
        "La lectura todavía no está estable."
      );
    }

    // 5. Comparar pH con referencias
    const ph = data.ph;

    if (ph > config.PH_MAX) {
      anomaly = "HIGH";
    }
    else if (ph < config.PH_MIN) {
      anomaly = "LOW";
    }
    else if (
      anomaly === "HIGH" &&
      ph <= config.PH_MAX - config.PH_HYSTERESIS
    ) {
      anomaly = null;
    }
    else if (
      anomaly === "LOW" &&
      ph >= config.PH_MIN + config.PH_HYSTERESIS
    ) {
      anomaly = null;
    }

    // 6. Anomalía activa
    if (anomaly !== null) {
      return result(
        "RED",
        "Anomalía detectada",
        "El pH presenta una anomalía respecto de la referencia."
      );
    }

    // 7. Zona cercana a los límites
    const nearLower =
      ph <= config.PH_MIN + config.PH_CAUTION_MARGIN;

    const nearUpper =
      ph >= config.PH_MAX - config.PH_CAUTION_MARGIN;

    if (nearLower || nearUpper) {
      return result(
        "YELLOW",
        "Precaución",
        "El pH está próximo a un límite de referencia."
      );
    }

    // 8. Sin anomalías detectadas
    return result(
      "GREEN",
      "Sin anomalía detectada",
      "El pH está dentro de la referencia preliminar."
    );
  }

  return { evaluate, reset };

})();
