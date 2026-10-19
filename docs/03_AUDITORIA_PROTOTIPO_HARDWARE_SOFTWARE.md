# Auditoría especializada — Construcción física y funcionamiento WaterAlpha

**Objetivo:** pauta técnica reproducible para convertir el software preparado en prototipo Alpha IoT, incluyendo cableado, adquisición, firmware, calibración, fallos y evidencia.  
**Fecha de corte:** 18-10-2026.  
**Advertencia:** sin placa ni hardware no existe demostración de mediciones reales; esta guía es **plan de implementación**, no esquema construido.  
**Documentos relacionados:** [Auditoría general](01_AUDITORIA_GENERAL_WATERALPHA.md) · [Protocolo de evidencias](06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md) · `06_mapa_electronico.md` / `07_protocolo_pruebas.md` del proyecto.

## 1. Requisitos de diseño y alcance Alpha

**Uso previsto:** senderistas que analizan una muestra de agua natural de manera **preliminar**. **No** consumo, certificación sanitaria ni detección de agentes microbiológicos. Subconjunto Alpha: temperatura °C y pH, alerta LED RGB, datos web offline. Conductividad eléctrica y turbidez quedan como expansión futura (no afirmar integradas).

**Arquitectura de referencia:**

```text
Agua y sondas en zona húmeda
   ├─ DS18B20 sumergible ─(1-Wire, pull-up a 3V3)──────────┐
   └─ sonda pH ─ módulo acondicionador AFEL ─ adaptación ──┤
                                          (ADC1)           ↓
                    Placa UAI con ESP-32S, área seca
                    WAMock o fuentes reales
                    → WAController / Snapshot
                    → WA::StateMachine
                          ├── WALed / LED RGB
                          └── HTTP GET /api/readings
                                      ↑
                            Wi-Fi SoftAP «WaterAlpha»
                                      ↑
                             navegador del usuario
```

### 1.1 Lista de materiales (BOM preliminar, sin inventar cantidades confirmadas)

| Material | Existencia | Propósito / decisión pendiente |
|---|---|---|
| Placa personalizada UAI + módulo Ai-Thinker ESP-32S | **No disponible todavía** | Medir PCB; obtener esquema y pinout, Flash, USB, reguladores y GPIO libres |
| Cable USB de datos + fuente USB 5 V adecuada | Por confirmar | Alimentación inicial de laboratorio, no asumir LiPo integrada |
| Sonda encapsulada DS18B20 + cable | Por confirmar | Medición °C; verificar fabricante, tres conductores y longitud |
| Resistencia pull-up ~4,7 kΩ | Por confirmar | Entre DQ y 3V3 de DS18B20; revisar presencia previa en módulo |
| Electrodo + placa pH analógica AFEL | Por confirmar | Registrar referencia, foto de ambos lados, tensión nominal, bornes y BNC |
| Resistencias de divisor/adaptador + multímetro | Por confirmar | Proteger ADC; dimensionar al observar rango e impedancias |
| LED RGB 4-terminales o módulo | Por confirmar | Confirmar común ánodo/cátodo y corriente admitida |
| Tres resistencias limitadoras LED | Por calcular | Una por color si LED discreto; valorar driver según corriente |
| Protoboard/cables, conectores, protección | Por confirmar | Ensayos secos, sin sonda sumergida sobre circuito expuesto |
| Buffer pH 4,00, 7,00 (y opcional 10,00); agua de enjuague | Por confirmar | Calibración y verificación, según rango esperado y sensor elegido |
| Termómetro de referencia | Por confirmar | Comparación DS18B20 a condiciones registradas |
| Recipiente o cámara de muestra, juntas/pasamuros | CAD existente reportado; piezas reales pendientes | Húmedo/seco, limpieza, cableado y mantenimiento |
| Computador Windows con Arduino IDE, VS Code, Python | Disponible en desarrollo | Compilar, grabar firmware y obtener logs |

**No afirmar IP67, autonomía, masa ni compatibilidad CAD validada** antes de medir y ensayar. Los ~Ø80 × 175–185 mm y cámara 100–130 mL son sugerencias del estudio previo, no cotas observadas del modelo real.

## 2. Gate de seguridad eléctrica previo a cablear

**GATE E1 — NO energizar conexiones hasta completar:**

- [ ] Fotografiar ambas caras de la PCB UAI; identificar `3V3`, `5V`/`VIN`, `GND`, USB, regulador, condensadores y pines libres. Confirmar el datasheet/pinout del ensamblaje, **no solo del chip ESP32**.
- [ ] Leer capacidad Flash real, particiones compatibles con LittleFS, USB/UART y control de arranque.
- [ ] Medir con multímetro las tensiones disponibles bajo carga y verificar polaridad antes de insertar sensores.
- [ ] Identificar nivel eléctrico de cada módulo y tierras comunes compatibles. No aplicar salida AFEL 5 V directamente al ADC del ESP32.
- [ ] Comprobar si el módulo AFEL entrega valores negativos, qué tensión máxima/mínima produce y su impedancia de salida. Un divisor resistivo **solo** es válido con señal positiva y condiciones eléctricas adecuadas; medir en vacío y bajo carga de prueba.
- [ ] Separar zona húmeda de placas, conectores y alimentación; evitar operación con líquidos sobre protoboard energizada.
- [ ] No usar pines de arranque, GPIO conectados a memoria Flash ni los dedicados a otros periféricos de la placa sin verificar.
- [ ] Dimensionar la corriente por GPIO y resistencias LED; comprobar fuente y consumo pico con Wi-Fi.

## 3. Mapa de conexiones — completar antes de soldar

**NO rellenar GPIO por conjetura:** `firmware/WaterAlpha/pins.h` define `-1` y `PINOUT_CONFIRMED=false` como protección. Los siguientes valores son *por determinar*.

| Dispositivo | Señal | Pin en módulo/placa UAI | Tensión / requisito | Verificación |
|---|---|---|---|---|
| DS18B20 | VDD | [POR CONFIRMAR] | 3,3 V previsto | Multímetro + ficha modelo |
| DS18B20 | GND | [POR CONFIRMAR] | Tierra | Continuidad |
| DS18B20 | DQ | GPIO digital [PENDIENTE] | Pull-up ~4,7 kΩ a 3V3 | Lectura de dirección 1-Wire |
| AFEL módulo | VCC/GND | [POR CONFIRMAR] | ~5 V nominal propuesto, validar | Inscripción + datasheet + multímetro |
| AFEL módulo | Analog out | ADC1 GPIO [PENDIENTE] | Tensión adaptada dentro de rango ADC admitido | Medida antes y después de adaptación |
| LED RGB | R/G/B | 3 GPIO PWM [PENDIENTES] | 3 resistencias/driver | V_f, I, común ánodo/cátodo |
| Fuente | USB/entrada | [POR CONFIRMAR] | alimentación acordada y capacidad de corriente | Medición bajo carga |

ADC1 en el chip ESP32 clásico puede disponer de GPIO 32–39 según encapsulado, pero **no todos están necesariamente disponibles en la placa UAI**, y ciertos GPIO tienen limitaciones de uso. Elegir a partir del esquema UAI y comprobar función de arranque. ADC2 puede entrar en conflicto con Wi-Fi: por ello preferir ADC1.

## 4. Secuencia de construcción física con criterios de salida

### Fase H0 — Inventario y medidas (sin corriente)

- Revisar y fotografiar BOM real, dimensiones con pie de metro, masa, configuración de cada módulo y etiquetas de cables.
- Contrastar envolventes 3D y agujeros con `Fusion 360`: altura real de PCB/módulos, conexiones BNC, radio de curvatura del cable, separación zonas húmeda/seca, accesos USB, pulsadores y salida de LED.
- Entrega: tabla de componentes y CAD actualizado; **criterio:** ningún componente atraviesa sólidos ni colisiona con otro, espacio para montaje/mantenimiento; registrar toda desviación.

### Fase H1 — Energía y placa (sin sensores)

- Conectar únicamente placa por USB confiable; verificar COM/driver y placa exacta (target de compilación provisional `ESP32 Dev Module` no valida pinout).
- Cargar firmware mediante Arduino IDE **solo después de confirmar hardware**; Monitor Serial 115200. Verificar mensajes `[BOOT]`, `[WIFI]`, `[HTTP]`, dirección IP y ausencia de reinicios.
- Cargar la imagen LittleFS como operación **separada**, seleccionando particiones compatibles con Flash real y herramienta `arduino-littlefs-upload`.
- Comprobar en navegador `GET /`, `style.css`, `app.js`, `/api/readings` desde Wi-Fi propio sin Internet; registrar headers, JSON y actualización.
- Entrega: `EVID-H1` (log serial, respuesta JSON, video corto); **criterio:** servidor funciona sin router, webpage íntegra y sin 404.

### Fase H2 — Solo DS18B20

- Desenergizar para cablear VDD-GND-DQ, pull-up a 3,3 V. Verificar con multímetro, colores del cable y continuidad según ficha específica.
- Identificar GPIO digital libre UAI; anotarlo en `pins.h` después de aprobar pinout, sin activar otros módulos.
- Modificar `WAConfig::USE_REAL_TEMPERATURE=true` únicamente cuando se haya revisado el cableado. Mantener `USE_REAL_PH=false` y `USE_REAL_LED=false` para aislar fallos.
- Verificar `OneWire` y `DallasTemperature`; primer ciclo de 12 bits puede tardar hasta ~750 ms y lectura inicial no válida. Registrar temperatura de muestra, tiempo, serial y comparación con termómetro.
- Probar desconexión de sonda en condiciones seguras; confirmar `temperature_sensor_ok:false`, `temperature:null`, estado `MAGENTA` y recuperación.
- **Criterios cuantitativos**: fijar antes de ensayo tolerancia frente a referencia y número de muestras; citar condiciones del DS18B20 elegido, no asumir precisión en todo el rango.

### Fase H3 — Solo AFEL analógico (empezar por voltaje)

- Confirmar la **referencia exacta** del kit AFEL y obtener ficha del módulo real; alimentar por tensión aprobada y común GND bajo procedimiento seguro.
- Medir salida analógica con multímetro **antes de conectar al ESP32**, en solución de referencia y estados de salida previsibles; registrar máximo, mínimo, polaridad y variación. No usar el dato del SEN0161-V2 DFRobot como si fuese del AFEL.
- Diseñar adaptación: si salida positiva y apropiada, elegir resistencias R1, R2 y considerar impedancia y filtrado; tensión en ADC: `V_ADC=V_OUT·R2/(R1+R2)`. Medir valor en el ADC bajo todas las condiciones y garantizar límites recomendados para atenuación seleccionada; 11 dB **no habilita 5 V**.
- Conectar solo ADC1 validado (Wi-Fi concurrente), sin sumergir la electrónica. Activar `USE_REAL_PH=true` solo para **registrar milivoltios**, manteniendo `signal_validated=false`, `calibration_verified=false` y estado MAGENTA; nunca etiquetar el número como pH todavía.
- Registrar ruido, saturación, frecuencia de muestreo y comportamiento de entrada abierta. Si un ADC «parece razonable» sin sonda, **no** inferir conexión real del electrodo.

### Fase H4 — Calibración pH real y validación

1. Confirmar instrucciones del fabricante, fecha/temperatura/estado de soluciones patrón; escoger por lo menos dos patrones que cubran el intervalo de operación (p. ej. pH 4 y 7; opcional verificación con 10 si corresponde).
2. Enjuagar electrodo con procedimiento adecuado, evitar contaminación cruzada, no secar abrasivamente membrana; almacenar según fabricante, no dejar secar si requiere solución de guarda.
3. Registrar `n` lecturas **en mV del ADC final** por cada patrón, a temperatura y tiempo de estabilización conocidos; guardar promedio, dispersión, deriva y serie cruda.
4. Para puntos `(V1,pH1)` y `(V2,pH2)`, calcular `m=(pH2-pH1)/(V2-V1)` y `b=pH1-m·V1`; la ecuación `pH=m·V_ADC+b` es una aproximación validable, no constante universal.
5. Comprobar interpolación y **solución independiente**; calcular error absoluto `|pH_estimado-pH_ref|`, sesgo y dispersión; definir un criterio aceptable técnicamente realista antes de ver el resultado.
6. Evaluar efecto de temperatura, envejecimiento y deriva; recalibrar según resultados. No afirmar compensación térmica porque exista un DS18B20 si el algoritmo no la implementa.
7. Implementar persistencia de `m`, `b`, fecha, rango y estado de verificación (NVS/Preferences o similar) con checksum/versionado y revalidación al iniciar. **Actualmente no está implementado.** No activar `calibration_verified` mediante un `true` de prueba: diseñar evento/procedimiento trazable con datos y registros.
8. Estudiar detección de electrodo desconectado/conector dañado con pruebas físicamente controladas: el retorno de ADC por sí solo NO lo detecta. Solo tras ese ensayo evaluar cuándo `signal_validated=true` es justificable.
9. Actualizar evidencia, CAD, esquema y versión del firmware; registrar quién calibró, con qué estándares y cuál error residual.

### Fase H5 — LED RGB y coherencia de salidas

- Identificar LED común ánodo o cátodo y niveles admitidos; calcular una resistencia **por canal** (`R≈(V_fuente−V_F)/I`, con margen y límites de la placa) y utilizar driver cuando corresponda; no activar pines para tantear.
- Confirmar tres GPIO realmente libres y compatibles con PWM/arranque; anotar en `pins.h`, `LED_TYPE_CONFIRMED=true` solo después de inspección y cálculo.
- Activar `USE_REAL_LED` únicamente tras pruebas estáticas sin agua, verificar color por estado BLUE, GREEN, YELLOW, RED, MAGENTA y coherencia con serial/web.
- Revisar si se requiere parpadeo MAGENTA físico: la versión descrita muestra MAGENTA sólido en LED, mientras la web puede animarlo; documentar como mejora pendiente.

### Fase H6 — Ensamble, batería y uso en campo

- Integrar sensores en cámara húmeda y electrónica en compartimento seco, con juntas y prensaestopas dimensionados según cableado real.
- Revisar ensamblaje CAD y fabricación/tolerancias; **no** atribuir protección IP67 por presencia de junta.
- Medir autonomía/consumo si se usa batería; USB de laboratorio no prueba portabilidad real.
- Probar llenado, medición, vaciado, limpieza y transporte sin contaminar la muestra; diferenciar protección a salpicadura de inmersión probada.

## 5. Auditoría del firmware — módulos y funciones clave

| Módulo/archivo | Responsabilidad | Entrada/salida | Riesgo a vigilar |
|---|---|---|---|
| `WaterAlpha.ino` | `setup()`, SoftAP y `loop()` | Wi-Fi y HTTP | IP real, reconexión, no bloquear loop |
| `config.h` | Umbrales, intervalos, flags | Constantes | Flags redundantes y valores no verificados |
| `pins.h` | GPIO pendientes | `-1`, flags confirmación | Cambios inseguros sin esquema UAI |
| `types.h` | `Readings`, `Decision`, `State` | Estructuras de dato | Compatibilidad entre módulos |
| `mock_sensors.*` | 12 escenarios ficticios | `WAMock::read(ms)` | No usar como evidencia de medición real |
| `state_machine.*` | Prioridades y memoria histéresis | `evaluate(Readings)` | `HIGH`/`LOW` chocan con macros Arduino; usar `ABOVE_MAX`/`BELOW_MIN` |
| `controller.*` | Coordinación 1 Hz + snapshot | `begin()`, `update()`, `snapshot()` | Duplicación de funciones, datos desactualizados |
| `temperature_sensor.*` | DS18B20 OneWire | °C + `sensor_ok` | Primera lectura, desconexión -127, reconexión |
| `ph_sensor.*` | ADC1 mV | `RawReading` | Voltaje inseguro, ADC no equivale a electrodo válido |
| `ph_calibration.*` | Ajuste dos puntos | `fitTwoPoint`, `estimatePh` | Pendiente sintética no usar en físico |
| `ph_pipeline.*` | Puerta de evidencia | `process(..., evidence)` | `signal_validated`/`calibration_verified` deben permanecer false sin ensayos |
| `led_colors.*` | Mapa cinco estados | `fromState()` | Matriz RGB y diferencia UI/físico |
| `led_rgb.*` | PWM de tres canales | `begin()`, `setState()` | GPIO, corriente, tipo de LED y core ESP32 v3 |
| `web_server.*` | HTTP + LittleFS + JSON v1 | GET `/`, `/api/readings` | FS separada, `String`, escapado JSON y `sample_age_ms` |
| `data/` | Web embebida | HTML/CSS/JS | Desincronización con `web/` |

### Máquina de estados (resumen auditable)

```text
si sensor/calibración no validado → MAGENTA
si inicializando → BLUE
si dato no finito/fuera del rango medible → MAGENTA
si inestable → YELLOW
si pH > 8,5 o < 6,5 → RED y guardar anomalía
si la anomalía previa no se ha recuperado 0,1 pH → RED
si 6,5–6,6 o 8,4–8,5 → YELLOW
resto → GREEN (sin anomalía preliminar)
```

Valores de pH y bandas son **provisionales de diseño**, no dictamen normativo. Temperatura contextual y condición de validez, no «temperatura de potabilidad». Histeresis usa memoria de `Anomaly` entre muestras.

### Contrato HTTP v1 esperado

```json
{
  "schema_version": 1,
  "mode": "MOCK",
  "ph": 7.2,
  "temperature": 17.8,
  "state": "GREEN",
  "stable": true,
  "ph_sensor_ok": true,
  "temperature_sensor_ok": true,
  "calibrated": true,
  "reading_valid": true,
  "sample_age_ms": 0,
  "message": "pH dentro de referencia.",
  "warning": "Evaluacion preliminar. No certifica potabilidad ni ausencia de patogenos."
}
```

`mode:"MOCK"` en firmware significa datos ficticios; `API_MODE:"REAL"` en web significa obtenerlos **mediante HTTP real**, no que las sondas midan realmente. Si pH/T no disponible, enviar `null` con diagnósticos coherentes; no serializar NaN como JSON. La fuente de autoridad del estado real debe ser el **ESP32**, no el navegador.

## 6. Matriz de pruebas mínimas con puertas de aceptación

| ID | En qué consiste | Evidencia real exigible | Estado al corte |
|---|---|---|---|
| SW-01 | FSM (umbrales, falla, histéresis) | Consola de `test_state_machine.exe`, código y commit | V-reportado |
| SW-02 | Calibración matemática sintética | `test_ph_calibration.exe` y entradas/esperados | D; log por archivar |
| SW-03 | Puerta pH | `test_ph_pipeline.exe` y aserciones | D; log por archivar |
| SW-04 | Flujo simulado cronológico | `test_system_flow.exe` 17 condiciones | V-reportado de la fase; log por archivar |
| SW-05 | LED mapa RGB | `test_led_colors.exe` 5 condiciones | D; confirmar ejecución |
| SW-06 | Arduino compila | Captura CLI/IDE indicando versión ESP32 core | D; conservar registro final |
| HTTP-01 | Interfaz web consume API Python | navegador + JSON `mode:MOCK` + video | V-reportado |
| FS-01 | Compilar imagen LittleFS | existencia `mklittlefs.bin` | V-reportado |
| HW-01 | SoftAP real + LittleFS | captura IP serial y navegador desde segundo equipo | P/B |
| HW-02 | DS18B20 con referencia | datos °C y comparación | P/B |
| HW-03 | pH ADC protegido, calibración real | mV/pH/buffers/error | P/B |
| HW-04 | LED físico y sincronía con web | registro de transición/LED | P/B |
| HW-05 | Fallos sensor y conexión | MAGENTA y recuperación segura | P/B |
| HW-06 | Sistema integrado ≥5 min | video/log continuo + datos sin interrupciones | P/B |
| MEC-01 | Ensamble/estanqueidad/mantenimiento | fotos, medidas y ensayo acordado | P/B |

**Criterios metrológicos numéricos pendientes:** establecer antes del ensayo y de acuerdo con las especificaciones exactas, sensibilidad y necesidades de la entrega. No inventar una precisión.

## 7. Riesgos técnicos detectados (no ocultarlos)

- **Construcción física aún no comenzada:** «software listo» no es «Alpha funcional» conforme a Avance #2.
- **Pinout UAI desconocido:** no habilitar GPIO `-1`, ni instalar por conjetura divisores/LED.
- **Módulo AFEL específico sin ficha/medición:** no extrapolar exactitud o rango de DFRobot.
- **Falta de persistencia/calibración verificada en firmware:** en `applyPhSource` el modelo es inválido y el `Evidence` es falso; activar modo físico de pH debería producir MAGENTA hasta completar esta función.
- **Entrada ADC desconectada:** `acquired=true` no significa electrodo conectado; se necesita prueba de señal.
- **Estabilidad:** la FSM recibe `stable` desde MOCK; todavía debe definirse algoritmo basado en serie de mediciones reales (media/mediana/dispersión, ventana, tiempo de estabilización).
- **Estrategia de error:** al perder medición mientras estaba RED, estudiar si mostrar MAGENTA con histórico de anomalía y no borrar información de riesgo. El orden actual muestra YELLOW en inestable y conserva memoria de anomalía.
- **API:** `sample_age_ms` necesita umbral máximo de dato obsoleto y gestión de reconexión; revisar caché/errores HTTP y validación JSON.
- **LED 6.7:** compilar e integrar con core 3.x y confirmar cinco casos; MAGENTA fijo vs parpadeante en UI.
- **Mecánica:** la base de referencia propone dimensiones y O-rings, pero no prueba sellado, masa, tolerancias ni compatibilidad con PCB real.

## 8. Recomendación de orden de desarrollo restante

**Ahora sin placa:** congelar versiones y pruebas; completar registro de dependencias, BOM, plano de banco, esquema a completar, CAD parametrizado con dimensiones conocidas de componentes; preparar buffers, referencia y equipo; confirmar objeto real con docente.  
**Cuando llegue placa:** E1 → H1 → H2 → H3 → H4 → H5 → H6. No intentar conectar todo y corregir a la vez.  
**Antes de entrega:** generar logs, fotos y video de medición → decisión → LED + web; mantener versión de firmware y CAD coherente.

## 9. Fuentes base

`Clase_TEI201_Sistemas_Electronicos_Control.pptx` (diapositivas sobre arquitectura, ADC, potencia, validación); `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx` (parámetros, interferencias, fabricación); `Base teórica y técnica...pdf` (pp. 1–13 como propuesta, no dato empírico del equipo); `avance_02_estudiantes.pdf` (pp. 2–8); documentación técnica de la conversación WaterAlpha. **Verificar la ficha del modelo AFEL/DS18B20 adquirido y del ESP32 UAI al disponer de componentes.**
