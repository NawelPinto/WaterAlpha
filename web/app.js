
/*
 * WaterAlpha - Control de interfaz
 * BLOQUE 2.2
 */

const mock = window.waterAlphaMock;

// Actualiza la información visible
function render() {

  const data = mock.getReadings();

  // Mostrar mediciones
  document.getElementById("ph-value").textContent =
    data.ph_sensor_ok ? data.ph.toFixed(2) : "--";

  document.getElementById("temperature-value").textContent =
    data.temperature_sensor_ok
      ? data.temperature.toFixed(1)
      : "--";

  // Diagnósticos
  document.getElementById("ph-sensor-status").textContent =
    data.ph_sensor_ok
      ? "✓ Sensor de pH: OK"
      : "✕ Sensor de pH: desconectado (simulado)";

  document.getElementById("temperature-sensor-status").textContent =
    data.temperature_sensor_ok
      ? "✓ Sensor de temperatura: OK"
      : "✕ Sensor de temperatura: desconectado (simulado)";

  document.getElementById("stability-status").textContent =
    data.stable
      ? "✓ Lectura estable"
      : "○ Lectura inestable";

  document.getElementById("calibration-status").textContent =
    data.calibrated
      ? "✓ Calibración válida (simulada)"
      : "✕ Calibración pendiente";

  // Estado provisional
  document.getElementById("status-title").textContent =
    "Simulación activa";

  document.getElementById("status-message").textContent =
    "Lecturas ficticias. Evaluación de estados pendiente.";
}

// Control deslizante de pH
document.getElementById("ph-input")
  .addEventListener("input", (event) => {

    const value = Number(event.target.value);

    mock.updateReadings({ ph: value });

    document.getElementById("ph-slider-value")
      .textContent = value.toFixed(2);

    render();
  });

// Control deslizante de temperatura
document.getElementById("temperature-input")
  .addEventListener("input", (event) => {

    const value = Number(event.target.value);

    mock.updateReadings({ temperature: value });

    document.getElementById("temperature-slider-value")
      .textContent = value.toFixed(1) + " °C";

    render();
  });

// Controles de diagnóstico
const switches = [
  ["ph-connected", "ph_sensor_ok"],
  ["temperature-connected", "temperature_sensor_ok"],
  ["stable-input", "stable"],
  ["calibrated-input", "calibrated"]
];

switches.forEach(([id, key]) => {

  document.getElementById(id)
    .addEventListener("change", (event) => {

      mock.updateReadings({
        [key]: event.target.checked
      });

      render();
    });

});

// Primera actualización
render();
