# Variables y diagnóstico WaterAlpha — recordatorio de exposición

## Parámetros medidos actualmente

| Variable | Símbolo | Unidad | Sensor y señal | Función exacta |
|---|---|---|---|---|
| **Potencial de hidrógeno** | `pH` | Sin unidad (adimensional) | Electrodo AFEL → módulo analógico → adaptación → ADC1 → calibración | Identificar la acidez/alcalinidad en relación con una referencia experimental de 6,5–8,5 |
| **Temperatura de la muestra** | `T` | °C | DS18B20 encapsulado → 1-Wire | Registrar temperatura y evaluar validez/estabilidad temporal; no atribuir un rango «potable» de temperatura |

**No medidos en esta versión:** conductividad eléctrica `CE` (µS/cm) y turbidez (NTU); tampoco se identifican microorganismos, metales ni contaminantes químicos específicos.

## Parámetros instrumentales y variables derivadas

| Variable | Unidad/tipo | Qué significa |
|---|---|---|
| `adcMillivolts` | mV | Tensión de AO ya adaptada en la entrada ADC1; base de calibración de pH |
| `m`, `b` | pH/mV, pH | Pendiente e intercepto de la recta experimental pH = m·mV + b |
| `stable` | Booleano | Ventanas de 5 lecturas cumplen el criterio de variación de voltaje y temperatura (solo en REAL después de calibrar) |
| `calibrated` | Booleano | Calibración pH aceptada por el pipeline bajo los requisitos de evidencia de campo |
| `reading_valid` | Booleano | La FSM validó entrada y estado de lectura; NO indica potabilidad |
| `temperature_sensor_ok`, `ph_sensor_ok` | Booleanos | Diagnósticos de subsistemas; no equivalen a exactitud ni ausencia de fallas invisibles |
| `mode` | MOCK/REAL | Origen simulado o adquisición desde hardware; REAL no implica calibración válida |
| `sample_age_ms` | ms | Edad de la última muestra respecto del instante de la consulta HTTP |
| `uptime_ms` | ms | Tiempo desde arranque de firmware, no fecha calendaria |
| `timestamp_local`, `iteration` | ISO8601, entero | Hora del PC e índice de sondeo guardados por `record_readings.py` |
| `state` | Categoría | BLUE/GREEN/YELLOW/RED/MAGENTA según prioridad de la FSM |

## Rangos y criterios

- **pH 6,5–8,5:** referencia interna preliminar del prototipo, no frontera universal entre agua potable y no potable. Margen amarillo ±0,1 pH y liberación por histéresis de 0,1 pH, según código compartido.
- **Temperatura:** rango físico de detección del DS18B20 -55 a 125 °C (según ficha del chip; no confundir con rango seguro de la sonda ni temperatura de muestra). La propuesta de estabilidad temporal usa ≤0,5 °C de amplitud en cinco lecturas, sujeto a validación.
- **Estabilidad de pH:** ventana de 5 lecturas de milivoltios, margen `STABILITY_MAX_SPAN_MV` por medir; un valor de campo de `-1` significa NO VALIDADO, no debe sustituirse por un valor arbitrario.

## Estados y patrones físicos

| Estado interno | Interfaz | Tres LED discretos | Significado correcto |
|---|---|---|---|
| `BLUE` | Inicio / cargando | Amarillo intermitente lento | Inicialización |
| `GREEN` | Verde | Verde fijo | pH dentro de referencia, temperatura registrada y ambas lecturas estables/validadas |
| `YELLOW` | Amarillo | Amarillo fijo | Inestabilidad o pH cercano a límites |
| `RED` | Rojo | Rojo fijo | pH fuera del intervalo experimental de referencia |
| `MAGENTA` | Error | Rojo intermitente rápido | Falta de calibración, lectura/sensor inválido u otra condición de falla |

**Frase para responder al profesor:** «El color indica el diagnóstico de mediciones fisicoquímicas muy acotadas. No estamos haciendo una prueba de potabilidad; para eso faltan otros parámetros y ensayos microbiológicos».

**Fuentes:** usar el `FUENTES.md` del equipo, la ficha de DS18B20 y especificación real del módulo AFEL. La correspondencia entre este resumen y el firmware debe demostrarse con capturas de la API y pruebas.