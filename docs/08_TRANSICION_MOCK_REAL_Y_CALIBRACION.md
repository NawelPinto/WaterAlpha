
## 2. Cambios manuales adicionales obligatorios

### config.h

Dentro de `namespace WAConfig`, **comprobar que existen** las lineas (no duplicarlas):

```cpp
constexpr bool MOCK_MODE = true;
constexpr bool USE_REAL_TEMPERATURE = false;
constexpr bool USE_REAL_PH = false;
constexpr bool USE_REAL_LED = false;
constexpr uint32_t SAMPLE_PERIOD_MS = 1000;
```

Las tres banderas `USE_REAL_*` corresponden a componentes fisicos. `MOCK_MODE` gobierna la fuente de datos global:

| Escenario | MOCK_MODE | TEMPERATURE | PH | LED |
|---|---|---|---|---|
| Computador / firmware sin hardware | true | false | false | false |
| ESP32 comprobando Wi-Fi, solo simulacion | true | false | false | false |
| Primera prueba DS18B20 | false | true | false | false |
| Lectura cruda ADC + temperatura | false | true | true | false |
| Sistema real validado | false | true | true | false |

**No hay modo hibrido en este controller:** con `MOCK_MODE=false`, un sensor deshabilitado aparece como **invalido**, nunca se sustituye por un dato MOCK. Durante el ensayo de solo temperatura, es normal que la FSM termine en MAGENTA, aunque se muestre la temperatura real. Eso evita presentar una decision GREEN apoyada en pH simulado.

### web_server.cpp

Buscar la instruccion actual que publica `"mode":"MOCK"` en el JSON, aproximadamente:

```cpp
json += "\"mode\":\"MOCK\",";
```

Reemplazar **esa sola instruccion** por:

```cpp
json += "\"mode\":\"";
json += (WAConfig::MOCK_MODE ? "MOCK" : "REAL");
json += "\",";
```

Verifica que `web_server.cpp` incluya `#include "config.h"`. Aqui `REAL` significa **fuente de adquisicion fisica**, no `reading_valid=true` ni agua potable. Si no existe evidencia suficiente, el JSON debe seguir indicando `calibrated:false`, `reading_valid:false` y/o estado MAGENTA.

### pins.h

No anotar GPIO inventados. Tras verificar la placa UAI, registrar `PIN_TEMP` (GPIO digital disponible) y `PIN_PH_ADC` (GPIO de **ADC1** disponible). Poner `PINOUT_CONFIRMED=true` solamente cuando esos pines hayan sido confirmados y sean seguros. Los tres pines LED RGB permanecen `-1`, `LED_TYPE_CONFIRMED=false` y `USE_REAL_LED=false`.

## 3. Datos a medir, NO a inventar

| Registro | Ejemplo de variable donde anotar | Metodo |
|---|---|---|
| Pin digital DS18B20 | `pins.h -> PIN_TEMP` | Pinout real de la placa UAI |
| Pin ADC1 pH | `pins.h -> PIN_PH_ADC` | Pinout y compatibilidad Wi-Fi |
| pH patron 1 | `BUFFER_1_PH` | Valor nominal del certificado corregido por temperatura |
| Promedio mV ADC en patron 1 | `BUFFER_1_ADC_MV` | Monitor Serial, varias lecturas estables |
| pH patron 2 | `BUFFER_2_PH` | Certificado de la solucion |
| Promedio mV ADC en patron 2 | `BUFFER_2_ADC_MV` | Monitor Serial |
| pH patron 3 (verificacion) | `CHECK_BUFFER_PH` | Tercer patron no usado para ajustar |
| Promedio mV ADC en patron 3 | `CHECK_BUFFER_ADC_MV` | Monitor Serial |
| Error maximo aceptado | `MAX_CHECK_ERROR_PH` | Criterio tecnico definido ANTES de ver resultados |
| Rango operativo seguro/validado de ADC | `ADC_VALID_MIN_MV / ADC_VALID_MAX_MV` | Revisar ficha, adaptador y ensayo sin saturacion |
| Amplitud de estabilidad admisible | `STABILITY_MAX_SPAN_MV` | Caracterizar dispersion de cinco mediciones consecutivas |
| Integridad de alimentacion/adaptacion comprobada | `ELECTRICAL_CHAIN_VERIFIED` | Medicion con multimetro; sin tension negativa en GPIO |
| Electrodo conectado, respuesta verificada | `PROBE_RESPONSE_VERIFIED` | Ensayo de respuesta a patrones, conexion y repetibilidad |
| Calibracion revisada con evidencia | `CALIBRATION_REVIEWED` | Resultado, error con tercer patron y trazabilidad |

**No escribir temperaturas ni pH de muestras en `controller.cpp`:** se leen en tiempo de ejecucion. Los voltajes de calibracion siempre se toman EN EL ADC ESP32, despues del adaptador electrico, con la misma configuracion de atenuacion y alimentacion prevista para las mediciones.

## 4. Secuencia de puesta en marcha con hardware

1. **Sin conectar el AFEL al ADC:** confirmar placa, alimentaciones, polaridad, valores de salida y ausencia de tension superior al permitido o negativa en el GPIO. El modulo puede requerir desplazamiento de nivel, no solo divisor; se decide con mediciones y documentacion del modulo real. No conectar 5 V al GPIO. Confirmar GND comun.
2. Mantener MOCK_MODE=true. Compilar, cargar firmware e imagen LittleFS por los procedimientos del ESP32. Confirmar que Wi-Fi local `WaterAlpha`, `GET /api/readings` y website funcionen con datos MOCK. El LED fisico sigue ausente.
3. Con DS18B20 conectado correctamente (3,3 V, GND, datos y pull-up ~4,7 kohm), configurar PIN_TEMP, MOCK_MODE=false, USE_REAL_TEMPERATURE=true, USE_REAL_PH=false. Recompilar y subir. Comprobar temperatura y sensor_ok por JSON/Serial. MAGENTA por falta de pH es esperado.
4. Tras validar adaptacion electrica y PIN_PH_ADC de ADC1, fijar USE_REAL_PH=true, conservar sentinelas de calibracion y banderas false. Subir firmware. Revisar por Monitor Serial a 115200 baud las lineas `[PH ADC REAL] ... mV`; el programa seguira sin aceptar una medicion de pH.
5. En tres alicuotas separadas de buffer (sugerencia 4,00; 10,00; 7,00), registrar multiples voltajes estables y temperatura. Enjuagar la sonda segun protocolo entre soluciones y no devolver liquido usado al frasco. Completar CSV y calcular promedio y rango de cada serie.
6. Anotar datos medidos y umbrales de aceptacion en `ph_field_data.h`. Primero permanecer con las tres banderas de verificacion en `false`: compilar y comprobar que el valor de pH REAL no se declare valido. Auditar el ensayo y **solo entonces** cambiar las banderas correspondientes a `true`.
7. En el arranque se calcula `pH = m * mV + b` con los dos patrones, se comprueba el error con el tercero y se habilita el pipeline unicamente si todos los controles pasan. Esperar por lo menos cinco muestras reales estables y una temperatura valida antes de GREEN/YELLOW/RED.
8. Comprobar escenarios repetibles: GREEN dentro de referencia; RED fuera de referencia, histéresis, perdida de temperatura, desconexion de sonda, fallas de ADC y reinicio. **La banda de voltaje y la estabilidad no garantizan detectar una sonda desconectada**; incluir pruebas de fallas reales y agregar diagnosticos especificos antes de declarar aceptada esa prueba.
9. Dejar USE_REAL_LED=false. El estado existe en la FSM, la API y el LED **virtual** del website; no hace falta LED fisico. Revisar por separado si la pauta academica exige un actuador fisico.

## 5. Indicadores del serial esperados

- `[CTRL] Modo MOCK...` significa que no se adquieren sensores fisicos.
- `[CTRL] Modo REAL...` significa que `WAMock::read` ya no se usa para las muestras.
- `[PH ADC REAL] <numero> mV` indica una adquisicion ADC; NO equivale por si sola a medicion valida de pH.
- `[PH CAL] ...` confirma que se intento validar la calibracion con datos registrados.
- `[STATE] MAGENTA` indica que la FSM considera invalida una condicion requerida.
- `[LED VIRTUAL] ...` se registra aunque no haya LED RGB; el website visualiza la decision de la FSM.

## 6. Comprobaciones previas y posteriores

Antes de conectar: correr `test_state_machine.exe`, `test_ph_calibration.exe`, `test_ph_pipeline.exe`, `test_system_flow.exe` y `test_led_colors.exe` (ver auditoria 05). Verificar compilacion en Arduino IDE 2, con `esp32 by Espressif Systems` de la rama 3.x y `ESP32 Dev Module` provisional hasta conocer la placa.

Despues de conectar: comprobar Serial, Wi-Fi SoftAP, JSON y website. Comparar temperatura con termometro de referencia y pH contra un patron **no utilizado en el ajuste**. Documentar capturas, logs, trazabilidad de soluciones, errores, repetibilidad, energia/cableado y version de firmware.

**ATENCION:** este codigo puede compilarse en un entorno que cumpla las interfaces previas, pero no constituye validacion de la placa fisica ni calibracion de hardware. La verificacion con el archivo `controller.h` y el resto de los archivos actuales del repositorio es obligatoria al integrar el paquete.

## 7. Pendientes de la etapa definitiva

- Confirmar el hardware exacto UAI, el ADC, la salida AFEL y su circuito de adaptacion.
- Calibracion de mas puntos o compensacion por temperatura, segun especificaciones y necesidades.
- Diagnostico continuo mas fuerte de electrodo desconectado y falla de alimentacion.
- Persistencia de calibracion en NVS/LittleFS si se pretende recalibracion sin recompilar.
- Ajustes de arranque, registro de incertidumbre, estabilidad y repetibilidad.
- Pruebas en distintas temperaturas y verificacion completa bajo Wi-Fi activo.
- Seguridad de acceso web y credenciales propias para una implementacion fuera de la demostracion.
