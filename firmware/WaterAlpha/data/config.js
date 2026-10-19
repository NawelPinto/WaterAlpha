
/*
 * WATERALPHA - CONFIGURACIÓN
 * Versión Alpha 0.1
 *
 * Todos los umbrales son preliminares.
 * NO certifican potabilidad.
 */

window.WaterAlphaConfig = Object.freeze({
  // Modo de funcionamiento 
  API_MODE: "REAL",
 
  // Actualización de mediciones
  API_REFRESH_MS: 1500,

  // Tiempo máximo de espera del ESP32
  API_TIMEOUT_MS: 3000,

  // Referencia preliminar de pH
  PH_MIN: 6.5,
  PH_MAX: 8.5,

  // Cercanía a un límite: precaución
  PH_CAUTION_MARGIN: 0.1,

  // Banda de histéresis
  PH_HYSTERESIS: 0.1,

  // Rango físico básico del pH
  PH_PHYSICAL_MIN: 0,
  PH_PHYSICAL_MAX: 14,

  // Rango nominal DS18B20
  TEMP_SENSOR_MIN: -55,
  TEMP_SENSOR_MAX: 125

});
