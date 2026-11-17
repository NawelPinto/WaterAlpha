# Protocolo experimental WaterAlpha — Avance 3

**Versión:** propuesta de evaluación V0.2 (16/11/2026). **Estado:** definido documentalmente; ejecución física aún por comprobar. **Rol:** completar el apartado obligatorio de testing del Avance 3; no es un informe de resultados.

## A. Objetivo de la validación

Demostrar de manera reproducible que WaterAlpha: (1) obtiene lecturas **reales** de pH y temperatura cuando está adecuadamente conectado y calibrado; (2) decide con una máquina de estados trazable; (3) expresa el mismo diagnóstico en tres LED físicos y una interfaz web local; (4) guarda series de datos con fecha/hora e iteración; y (5) mantiene operación integrada bajo condiciones controladas. No se pretende evaluar patógenos, pesticidas, metales, salinidad ni certificar potabilidad.

## B. Variables y parámetros

**Variables medidas dependientes:** `pH` (adimensional) y `temperature` (°C). **Señal primaria analógica:** pH del módulo AFEL expresado como milivoltios en la entrada ADC1 después del adaptador (`adcMillivolts`). **Variables experimentales independientes:** patrón nominal pH (p. ej., buffers certificados 4/7/10), condición térmica de la muestra en tres niveles y evento de desconexión controlado. **Variables controladas:** fuente USB, volumen de muestra, profundidad de inmersión, tiempo de estabilización, Wi-Fi, tensión AFEL, ADC1, parámetros y versión de firmware, recipiente, limpieza del electrodo y temperatura de patrones.

**Indicadores de desempeño:** error absoluto `|medición - referencia|`, error promedio absoluto MAE, sesgo, rango máximo–mínimo de ventana temporal, tiempo hasta actualización/alarma, porcentaje de registros válidos, porcentaje de coincidencia LED–web, continuidad de conexión durante 300 segundos, completitud de CSV/JSONL. `reading_valid`, `calibrated`, `stable`, `sample_age_ms`, `mode` son **indicadores de calidad/diagnóstico**, no parámetros fisicoquímicos adicionales.

## C. Equipos y condiciones previas

Placa UAI Ai-Thinker ESP-32S, DS18B20, módulo/electrodo AFEL identificado, tres LED individuales con resistencias propias, cableado verificado, ordenador con Python/Arduino IDE, recipiente separado y multímetro. Para validar cuantitativamente se requieren soluciones tampón de pH con lote/vencimiento y especificación de temperatura, termómetro independiente y acceso al dashboard. No basta con agua de llave como «patrón» de pH.

Antes de aplicar alimentación: inspeccionar pinout de la PCB UAI, conectar GND común cuando corresponda, verificar 3,3 V y 5 V disponibles, medir AO de AFEL **sin conectarlo al ESP32**, dimensionar adaptación, probar ausencia de sobrevoltajes y confirmar resistencias y polaridad de cada LED. Mantener protoboards y equipos en superficie seca, **sin agua sobre placas energizadas**. Documentar foto y valores de voltaje. Si falla cualquiera de estas verificaciones, suspender pruebas físicas.

## D. Procedimiento secuencial reproducible

### T01 — Arranque, Wi-Fi local y HTTP

1. Registrar commit, versión de Arduino ESP32 Core, bibliotecas y alimentación USB.
2. Cargar firmware y por separado la imagen LittleFS, tras verificar Flash/partición.
3. Abrir Serial a 115200, conservar SSID/IP, comprobar carga de `index.html` y recursos.
4. Conectarse al SoftAP sin Internet y consultar `/api/readings` 20 veces; confirmar JSON `schema_version=1` y fuente `mode` correcta.
5. **Criterio previo:** 20 respuestas HTTP 200 con JSON interpretable y sin reboots; indicar si la fuente es simulada. Guardar log/capturas. No equivale a sensor calibrado.

### T02 — Termometría DS18B20

1. Sin alimentación, conectar VDD 3V3, GND y DATA con pull-up 4,7 kΩ al pin comprobado. Realizar medición de continuidad/voltaje; reenergizar.
2. Activar solo temperatura real (`MOCK_MODE=false`, `USE_REAL_TEMPERATURE=true`, `USE_REAL_PH=false`). Se espera estado general inválido por falta de pH, aunque temperatura sea real.
3. Comparar con termómetro independiente en tres condiciones térmicas seguras y estables, sugeridas 15, 25 y 35 °C, **si pueden acondicionarse correctamente**. Registrar 20 lecturas por condición, referencia, tiempo, lote de agua, profundidad y variación.
4. **Criterio de diseño PROVISIONAL:** error absoluto promedio ≤1,0 °C en las condiciones de prueba, sin `-127 °C` ni `85 °C` inicial inválido como dato aceptado; modificar criterio antes del ensayo si la referencia/objetivo exige otra tolerancia. Confirmar especificación del DS18B20 exacto.

### T03 — Seguridad ADC y caracterización AFEL

1. Confirmar modelo de módulo. Medir alimentación, AO en vacío y con buffer, rango de voltaje/polaridad y cambios ante alteración pH. Registrar valores/autor/fecha.
2. Calcular y montar divisor o acondicionador solo si la señal es compatible. Medir milivoltios realmente presentes en el GPIO ADC1 y comprobar que permanecen dentro del rango **recomendado** de Espressif para la atenuación elegida.
3. Activar adquisición pH solo tras comprobar tensión. Registrar 20 valores de ADC por solución. **No** declarar pH medido mientras no exista ajuste y verificación física.
4. **Criterio previo:** sin tensión negativa ni sobretensión en ADC; sin saturación, y respuesta reproducible a distintos patrones. Ante señal fuera de rango: detener y corregir circuito.

### T04 — Calibración de pH y comprobación independiente

1. Utilizar 2 buffers diferentes para obtener `pH = m * ADC_mV + b` (p. ej. pH 4,00 y 10,00), registrar 20 lecturas estables de cada uno, temperatura, promedio y dispersión.
2. Verificar con un **tercer patrón no utilizado** en el ajuste (pH 7,00 en este ejemplo), con otras 20 lecturas independientes. No usar la tercera solución para reajustar y luego reclamarla como control independiente.
3. Calcular `m=(pH2-pH1)/(mV2-mV1)`, `b=pH1-m·mV1`, error absoluto máximo/promedio y dispersión. Calibrar usando voltajes medidos en **entrada ADC** y no los voltajes nominales del electrodo.
4. **Criterio de diseño PROVISIONAL:** error absoluto en el tercer patrón ≤0,30 pH; revisar ficha, tolerancia de soluciones y necesidades del usuario antes de aprobarlo formalmente. Si no se dispone de buffers/verificación, `CALIBRATION_REVIEWED=false` y no activar la decisión GREEN basada en pH real.
5. Conservar fecha/lote/concentraciones, fuente de voltajes y resultados sin inventar. Documentar el reinicio y la persistencia o limitación de parámetros en `ph_field_data.h`.

### T05 — Estabilidad de señal y respuesta de estados

1. Con sensores físicamente verificados, tomar 5 lecturas consecutivas cada ~1 s.
2. Evaluar amplitud de ADC pH contra `WAField::STABILITY_MAX_SPAN_MV` **determinado con evidencia de ruido antes de validar**; evaluar amplitud de temperatura contra referencia interna provisional de 0,5 °C en cinco lecturas.
3. Probar pH dentro (verde), cercano a 6,5/8,5 (amarillo) y fuera de los límites (rojo) solo mediante patrones adecuados. Usar prueba simulada para inducir valores no disponibles; rotular MOCK claramente.
4. Probar histéresis con datos sintéticos de ambos límites. Para parte física, usar muestras/patrones independientes y evitar forzar un valor exacto arbitrario.
5. **Criterio:** cinco escenarios de estado reproducibles en lógica; sin pulsos inesperados cerca del umbral; no mostrar verde si falta calibración o sensores. Registrar tiempo y capturas.

### T06 — Correspondencia de LED físicos y dashboard

1. Con GPIO confirmados, tres resistencias limitadoras e inspección de polaridad, activar los LED físicos.
2. Para cada uno de los cinco estados internos, comparar texto/color web con el patrón físico: `GREEN`→verde fijo, `YELLOW`→amarillo fijo, `RED`→rojo fijo, `BLUE`→amarillo intermitente lento, `MAGENTA`→rojo intermitente rápido.
3. **Criterio:** correspondencia 100 % en 10 transiciones verificables y ausencia de calentamiento/fallo; registrar video con LED y pantalla simultáneos. Si solo se prueban escenarios MOCK, no presentarlos como detección física de pH.

### T07 — Registro persistente e integración durante 300 segundos

1. Ajustar la fecha y hora del PC, conectarlo al SoftAP e iniciar `python scripts/record_readings.py --url http://IP_REAL/api/readings --interval 2`.
2. Mantener el montaje estable por **300 s** y observar HTTP, LED, mediciones, `mode`, `sample_age_ms` y estado. Registrar incidentes, voltaje de fuente y desconexiones si ocurren.
3. Cerrar con Ctrl+C y abrir los CSV/JSONL. Contar filas, intervalos, valores nulos, errores, estados y modo. Extraer al menos un gráfico temporal de pH/°C o una tabla interpretada.
4. **Criterio propuesto:** 300 s sin reinicios no previstos, ≥98 % de las solicitudes respondidas exitosamente, coincidencia de estado LED/web en los eventos observados, archivos legibles con 100 % de filas fechadas, y distinción inequívoca de MOCK/REAL. Si una lectura falla, mantenerla como evento con valor vacío; no sustituir por datos simulados.

### T08 — Fallas deliberadas y recuperación

1. Ejecutar primero con el equipo desenergizado los cambios de conexión cuando sea necesario. No provocar cortocircuitos ni contacto con agua en placa.
2. Examinar desconexión digital DS18B20, ausencia de pH calibrado, pérdida de conexión HTTP y respuesta de reinicio de la placa.
3. **Criterio:** el sistema presenta `MAGENTA`/error o aviso equivalente y no reutiliza una lectura inválida como si fuera actual; log registra incidencia. Confirmar que detección de electrodo abierto no está resuelta simplemente por tener ADC dentro de rango.

## E. Tratamiento de datos y representación

Para n muestras con referencia `x_ref,i`, definir `error_i=x_i−x_ref,i`, `MAE=(1/n)Σ|error_i|`, `sesgo=(1/n)Σerror_i`, y si se desea `RMSE=√((1/n)Σerror_i²)`. Registrar n, unidades (pH adimensional / °C), referencia, fecha y condiciones.

Gráficos sugeridos: pH(t) con líneas 6,5/8,5 **etiquetadas como referencias del proyecto**, temperatura(t) y referencia externa, dispersión de mV por solución, estados vs tiempo, transiciones LED. Comparar tiempos de adquisición y error HTTP. La temperatura no se etiqueta «potable» a partir de ninguna temperatura puntual.

## F. Evidencia y formato de resultados

Crear `docs/evidencias/` con subcarpetas `hardware/`, `calibracion/`, `http/`, `demo/`, `cad/` y fotografías de al menos **tres ángulos**. Cada archivo debe asociarse a: `ID`, fecha, operador, commit, condición, instrumento, criterio previo, resultado observado, error, foto/log y veredicto `APROBADO/FALLIDO/PENDIENTE`. Guardar CSV sin alterar los datos en bruto. No inventar error porcentual con base de pH = 0, ni mezclar resultados MOCK con REAL.

| ID | Ensayo | Estado al redactar protocolo | Evidencia prevista |
|---|---|---|---|
| T01 | SoftAP + HTTP | PENDIENTE DE PRUEBA FÍSICA | Serial, URL, respuesta JSON |
| T02 | DS18B20 | PENDIENTE | Tabla °C vs referencia |
| T03 | ADC/AFEL | BLOQUEADO POR VERIFICACIÓN ELÉCTRICA | mV y fotos multímetro |
| T04 | Calibración pH | BLOQUEADO POR BUFFERS Y MEDICIÓN | Tabla y ecuación con error |
| T05 | Estados/estabilidad | SIMULACIÓN PREPARADA; REAL PENDIENTE | Test C++, serie real |
| T06 | Semáforo LEDs | PENDIENTE DE GPIO Y RESISTENCIAS | Video simultáneo |
| T07 | Persistencia + 300s | SCRIPT PROBADO EN MOCK HTTP LOCAL; REAL PENDIENTE | CSV/JSONL reales |
| T08 | Fallas | PENDIENTE | Logs, pruebas controladas |

Los porcentajes de la pauta del Avance 3 valoran software 30 %, hardware 30 %, CAD 20 %, testing 10 %, GitHub 5 % y exposición 5 %. Aunque los resultados experimentales definitivos puedan quedar pendientes, este protocolo debe presentarse completo.