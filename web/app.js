
/*
 * WATERALPHA - INTERFAZ WEB
 * Bloque 4
 */

const api = window.WaterAlphaAPI;
const mock = window.waterAlphaMock;
const config = window.WaterAlphaConfig;

const element = id => document.getElementById(id);

let updating = false;

// Ocultar simulador si usamos hardware real
document.querySelector(".simulator").hidden =
  config.API_MODE !== "MOCK";

// Mostrar una respuesta válida
function render(data) {

  element("mode-badge").textContent =
    data.mode === "MOCK"
      ? "MODO SIMULACIÓN"
      : "DISPOSITIVO REAL";

  // Estado entregado por la API
  element("status-light").className =
    "status-light " + data.state.toLowerCase();

  element("status-title").textContent =
    data.state === "GREEN"
      ? "Sin anomalía detectada"
      : data.state === "RED"
        ? "Anomalía detectada"
        : data.state === "YELLOW"
          ? "Precaución"
          : data.state === "BLUE"
            ? "Inicializando"
            : "Error del sistema";

  element("status-message").textContent =
    data.message;

  // Mediciones
  element("ph-value").textContent =
    data.ph === null ? "--" : data.ph.toFixed(2);

  element("temperature-value").textContent =
    data.temperature === null
      ? "--"
      : data.temperature.toFixed(1);

  // Diagnósticos
  element("ph-sensor-status").textContent =
    data.ph_sensor_ok
      ? "✓ Sensor de pH: OK"
      : "✕ Sensor de pH: error";

  element("temperature-sensor-status").textContent =
    data.temperature_sensor_ok
      ? "✓ Sensor temperatura: OK"
      : "✕ Sensor temperatura: error";

  element("stability-status").textContent =
    data.stable
      ? "✓ Lectura estable"
      : "○ Lectura inestable";

  element("calibration-status").textContent =
    data.calibrated
      ? "✓ Calibración válida"
      : "✕ Calibración pendiente";

  // Inspector JSON
  element("json-preview").textContent =
    JSON.stringify(data, null, 2);
}

// Fallo de comunicación
function showConnectionError(error) {

  element("mode-badge").textContent =
    "SIN CONEXIÓN";

  element("status-light").className =
    "status-light offline";

  element("status-title").textContent =
    "Sin conexión";

  element("status-message").textContent =
    "No se pudo obtener una lectura actualizada.";

  element("ph-value").textContent = "--";
  element("temperature-value").textContent = "--";

  [
    "ph-sensor-status",
    "temperature-sensor-status",
    "stability-status",
    "calibration-status"
  ].forEach(id => {
    element(id).textContent = "— Sin datos recientes";
  });

  element("json-preview").textContent =
    "Error: " + error.message;
}

// Obtener datos a través de la API
async function refresh() {

  if (updating) return;

  updating = true;

  try {
    const data = await api.getReadings();
    render(data);

  } catch (error) {
    console.error("[WaterAlpha]", error);
    showConnectionError(error);

  } finally {
    updating = false;
  }
}

// Slider de pH
function registerSlider(id, key, outputId, decimals, unit = "") {

  element(id).addEventListener("input", event => {

    const value = Number(event.target.value);

    mock.updateReadings({ [key]: value });

    element(outputId).textContent =
      value.toFixed(decimals) + unit;

    refresh();
  });
}

registerSlider("ph-input", "ph", "ph-slider-value", 2);

registerSlider(
  "temperature-input",
  "temperature",
  "temperature-slider-value",
  1,
  " °C"
);

// Casillas del simulador
const switches = [
  ["ph-connected", "ph_sensor_ok"],
  ["temperature-connected", "temperature_sensor_ok"],
  ["stable-input", "stable"],
  ["calibrated-input", "calibrated"],
  ["initializing-input", "initializing"]
];

switches.forEach(([id, key]) => {

  element(id).addEventListener("change", event => {

    mock.updateReadings({
      [key]: event.target.checked
    });

    refresh();
  });
});

// Actualización automática
setInterval(refresh, config.API_REFRESH_MS);

// Primera consulta
refresh();
