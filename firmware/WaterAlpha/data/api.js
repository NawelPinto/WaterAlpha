
/*
 * WATERALPHA - CAPA DE COMUNICACIÓN
 * Bloque 4
 */

window.WaterAlphaAPI = (() => {

  const config = window.WaterAlphaConfig;

  const WARNING =
    "Evaluación preliminar. No certifica potabilidad ni ausencia de patógenos.";

  // 1. Construir la respuesta simulada
  function getMockResponse() {

    const raw = window.waterAlphaMock.getReadings();

    const decision =
      window.WaterAlphaStateMachine.evaluate(raw);

    const phValid =
      raw.ph_sensor_ok === true &&
      raw.calibrated === true &&
      Number.isFinite(raw.ph) &&
      raw.ph >= config.PH_PHYSICAL_MIN &&
      raw.ph <= config.PH_PHYSICAL_MAX;

    const tempValid =
      raw.temperature_sensor_ok === true &&
      Number.isFinite(raw.temperature) &&
      raw.temperature >= config.TEMP_SENSOR_MIN &&
      raw.temperature <= config.TEMP_SENSOR_MAX;

    return {
      schema_version: 1,
      mode: "MOCK",

      temperature: tempValid ? raw.temperature : null,
      ph: phValid ? raw.ph : null,

      state: decision.state,
      stable: raw.stable === true,

      temperature_sensor_ok:
        raw.temperature_sensor_ok === true,

      ph_sensor_ok:
        raw.ph_sensor_ok === true,

      calibrated: raw.calibrated === true,

      reading_valid:
        phValid &&
        tempValid &&
        raw.stable === true &&
        !raw.initializing,

      sample_age_ms: 0,

      message: decision.message,
      warning: WARNING
    };
  }

  // 2. Consultar el servidor del ESP32
  async function getRealResponse() {

    const controller = new AbortController();

    const timer = setTimeout(() => {
      controller.abort();
    }, config.API_TIMEOUT_MS);

    try {
      const response = await fetch("/api/readings", {
        signal: controller.signal,
        cache: "no-store"
      });

      if (!response.ok) {
        throw new Error(
          "Error HTTP: " + response.status
        );
      }

      return await response.json();

    } finally {
      clearTimeout(timer);
    }
  }

  // 3. Validación básica del contrato
  function validate(data) {

    const states = [
      "BLUE", "GREEN", "YELLOW", "RED", "MAGENTA"
    ];

    if (
      !data ||
      data.schema_version !== 1 ||
      !states.includes(data.state) ||
      !["MOCK", "REAL"].includes(data.mode)
    ) {
      throw new Error("Formato de API inválido");
    }

    const validNumber = value =>
      value === null ||
      (typeof value === "number" && Number.isFinite(value));

    if (
      !validNumber(data.ph) ||
      !validNumber(data.temperature)
    ) {
      throw new Error("Medición numérica inválida");
    }

    const booleanFields = [
      "stable",
      "temperature_sensor_ok",
      "ph_sensor_ok",
      "calibrated",
      "reading_valid"
    ];

    if (booleanFields.some(
      key => typeof data[key] !== "boolean"
    )) {
      throw new Error("Diagnósticos inválidos");
    }

    if (
      typeof data.message !== "string" ||
      typeof data.warning !== "string" ||
      !Number.isFinite(data.sample_age_ms) ||
      data.sample_age_ms < 0
    ) {
      throw new Error("Metadatos inválidos");
    }

    return data;
  }

  // 4. Punto único de acceso para la interfaz
  async function getReadings() {

    const data = config.API_MODE === "MOCK"
      ? getMockResponse()
      : await getRealResponse();

    return validate(data);
  }

  return { getReadings };

})();
