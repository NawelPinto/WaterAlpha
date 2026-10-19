#ifndef WATERALPHA_PH_FIELD_DATA_H
#define WATERALPHA_PH_FIELD_DATA_H

#include <stdint.h>

// DATOS DE CAMPO: reemplazar UNICAMENTE valores medidos y verificados.
// -1.0 indica "sin medir": el sistema rechaza el pH real.
// Todos los voltajes son mV LEIDOS POR EL ADC del ESP32, DESPUES
// de la adaptacion electrica del modulo AFEL.
namespace WAField {

// Dos soluciones patron para construir la recta pH = m * mV + b.
// Modifica el pH nominal si usas otras soluciones.
constexpr double BUFFER_1_PH = 4.00;
constexpr double BUFFER_1_ADC_MV = -1.0; // ANOTAR promedio medido

constexpr double BUFFER_2_PH = 10.00;
constexpr double BUFFER_2_ADC_MV = -1.0; // ANOTAR promedio medido

// Tercera solucion patron para comprobar el resultado, NO ajustar.
constexpr double CHECK_BUFFER_PH = 7.00;
constexpr double CHECK_BUFFER_ADC_MV = -1.0; // ANOTAR promedio medido

// Error absoluto maximo aceptable para ese punto de comprobacion.
// Establecer segun especificaciones, incertidumbre y criterio de ensayo.
constexpr double MAX_CHECK_ERROR_PH = -1.0; // ANOTAR criterio aprobado

// Rango operativo admisible en la ENTRADA DEL ADC, medido y seguro.
// NO es el rango de alimentacion del sensor.
constexpr double ADC_VALID_MIN_MV = -1.0; // ANOTAR limite validado
constexpr double ADC_VALID_MAX_MV = -1.0; // ANOTAR limite validado

// Umbral de estabilidad: amplitud max-min observada en la ventana
// de cinco muestras consecutivas a un segundo. Es un criterio
// empirico, no una especificacion de precision del electrodo.
constexpr double STABILITY_MAX_SPAN_MV = -1.0; // ANOTAR criterio validado
constexpr uint8_t STABILITY_SAMPLES = 5;

// Confirmaciones de ensayo. Mantener FALSE hasta contar con evidencia.
// Un true NO sustituye controles de desconexion en tiempo real.
constexpr bool ELECTRICAL_CHAIN_VERIFIED = false;
constexpr bool PROBE_RESPONSE_VERIFIED = false;
constexpr bool CALIBRATION_REVIEWED = false;

// Tiempo de arranque inicial (no significa que el sensor ya sea valido).
constexpr uint32_t REAL_STARTUP_MS = 3000;

} // namespace WAField

#endif
