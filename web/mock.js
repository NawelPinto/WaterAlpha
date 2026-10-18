
/*
 * WaterAlpha - Simulador de sensores
 * BLOQUE 2.2
 *
 * Estos datos son ficticios.
 * NO provienen de sensores reales.
 */

const simulatedReadings = {
  mode: "MOCK",

  ph: 7.2,
  temperature: 17.8,

  ph_sensor_ok: true,
  temperature_sensor_ok: true,

  stable: true,
  calibrated: true,

  state: "BLUE"
};

// Proveedor de lecturas simuladas
window.waterAlphaMock = {

  getReadings() {
    return { ...simulatedReadings };
  },

  updateReadings(changes) {
    Object.assign(simulatedReadings, changes);
  }

};
