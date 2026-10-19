# Auditoría general — WaterAlpha (TEI201, UAI)

**Fecha de corte:** 18-10-2026 · **Naturaleza:** diagnóstico de madurez, brechas y conformidad académica.  
**Alcance:** diseño, CAD, electrónica, software, metrología, testing, demostración, paper, reproducibilidad.  
**No sustituye** una revisión de código del repositorio, ensayo de laboratorio ni evaluación del docente. Véase [00_LEEME_AUDITORIAS.md](00_LEEME_AUDITORIAS.md).

## 1. Dictamen ejecutivo

**Dictamen:** preparación de software **avanzada, con evidencias parciales de compilación y pruebas de lógica**, pero **sin demostración de un prototipo físico integrado**. La principal brecha para las exigencias académicas no es añadir más archivos C++; es obtener hardware, identificar el pinout y los niveles eléctricos, adquirir y calibrar mediciones reales, y registrar una demo integral de sensor–procesamiento–LED–web.

**Separación obligatoria de lenguaje:**

- `Compila`: el compilador de Arduino no reporta errores para el objetivo provisional `ESP32 Dev Module`.
- `Prueba unitaria superada`: se ejecutó un caso de lógica en Windows, con entradas controladas.
- `Prueba HTTP simulada superada`: la web procesó JSON entregado por Python.
- `Verificado en ESP32`: requiere ejecutar realmente el firmware cargado en una placa y observar resultados.
- `Validado metrológicamente`: requiere referencia, procedimiento, incertidumbre/error y datos reales.

No utilizar un estado de madurez como sinónimo de otro.

## 2. Cuatro entregables definidos por el equipo

| Entregable | Evidencia actual reportada | Estado auditor | Condición de cierre |
|---|---|---|---|
| A. Modelo/prototipo 3D Fusion 360 | Equipo declara que existe | **D**: existencia reportada, no se inspeccionó archivo | Abrir ensamble nativo, capturar vistas, acotar dimensiones, comprobar espacio para placa y sondas reales, exportar evidencias |
| B. PPT y presentación oral | Pauta de Avance #2 disponible, no existe PPT auditado | **P** | Construir 5–6 láminas útiles, guion y demo ensayada en 5–7 min; verificar requerimiento concreto de PPT con docente |
| C. Informe Entrega 2 bajo instructivo APA | Instructivo y plantilla revisados, texto final no auditado | **P** | Manuscrito PDF con pruebas, CAD, esquemático, discusión, referencias, autorías, anexos y límites declarados |
| D. Mostrar funcionamiento en website | Interfaz MOCK y prueba HTTP con servidor Python reportadas como exitosas | **D/B**: demostración simulada preparada; no dispositivo real | ESP32 sirviendo la web por SoftAP, leyendo sensores, accionando LED; demostrar estabilidad mínima 5 min |

**Observación cronológica:** `avance_02_estudiantes.pdf` especifica una evaluación celebrada/programada para el **12 de octubre de 2026**. La descripción de «cuatro entregables de Entrega 2» proviene del equipo y puede corresponder a otra instancia. **Confirmar con el docente cronograma, exigencia de PPT, plazo y reglas de demo**; no fusionar automáticamente ambas pautas.

## 3. Inventario de subsistemas y madurez

| Subsistema | Estado / evidencia | Brecha crítica |
|---|---|---|
| Frontend (`web/index.html`, `style.css`, `app.js`) | **D:** interfaz oscura, LED virtual, pH/T, diagnóstico, aviso permanente; validación visual reportada | Usabilidad con celular, accesibilidad, caché y reconexión en SoftAP real |
| Configuración web (`web/config.js`) | **D:** `API_MODE: "MOCK"`, refresco 1500 ms, timeout 3000 ms descritos | Sincronizar con copia en `firmware/WaterAlpha/data/` |
| API del navegador (`web/api.js`) | **D:** `GET /api/readings` y validador JSON; HTTP Python reportado | Error de red, respuesta incompleta, envejecimiento de muestras, test en ESP32 |
| FSM JavaScript | **D:** cinco estados con umbrales/histéresis | Evitar dos autoridades de decisión cuando el firmware está en REAL |
| FSM C++ (`state_machine.*`) | **V-reportado:** `test_state_machine.exe` finalizó `pruebas OK` | Paridad JS/C++ con casos límite, regresión y firmware real |
| Controlador (`controller.*`) | **D:** Snapshots, máquina única, actualización 1 s | Ensayo de integración real, comportamiento de sensores lentos y estado obsoleto |
| Sensores simulados (`mock_sensors.*`) | **D:** secuencia de 12 escenarios | Asegurar que toda evidencia declare `mode: MOCK` |
| Servidor SoftAP/HTTP (`WaterAlpha.ino`, `web_server.*`) | **D:** compilado y rutas previstas `/`, `/api/readings` | Sin carga ni prueba en ESP32; revisar JSON escapado/errores HTTP |
| LittleFS (`data/`) | **V-reportado:** imagen `mklittlefs.bin` generada | Carga a partición real correcta; sincronización con fuente `web/` |
| DS18B20 (`temperature_sensor.*`) | **D:** librerías OneWire/DallasTemperature, ruta asincrónica prevista | Sensor, pin, alimentación, pull-up, detección de desconexión, error térmico |
| pH AFEL (`ph_sensor.*`) | **D:** adquisición `analogReadMilliVolts` tras protección por flags | Ficha exacta, señal eléctrica, ADC1, divisor/adaptación, metrología |
| Calibración (`ph_calibration.*`) | **D:** recta de dos puntos y pruebas sintéticas propuestas | Coeficientes reales, persistencia, verificación independiente, temperatura |
| Filtro de validez (`ph_pipeline.*`) | **D:** exige evidencia de señal y calibración, actualmente falsa | Implementar adquisición verificable y habilitación bajo procedimiento |
| LED RGB (`led_colors.*`, `led_rgb.*`) | **D/P:** módulos y tests descritos; no hay resultado explícito final auditado del bloque 6.7 | Confirmar compilación/pruebas, tipo de LED, resistencias, GPIO, montaje |
| CAD | **D:** Fusion 360 declarado terminado | No se midió interferencia, escala, cierres, cámara húmeda, volumen ni fijaciones |
| Esquemático real | **P/B:** mapa conceptual sin pines | Pinout y alimentación antes de dibujar el circuito definitivo |
| Documentación de ensayos | **D:** borradores `06_mapa_electronico.md`, `07_protocolo_pruebas.md` | Completar criterios de aceptación medibles y resultados trazables |

*La calificación D no certifica que el archivo exacto exista en la última versión de GitHub: el equipo debe contrastarlo con el árbol real del repositorio.*

## 4. Requisitos funcionales y no funcionales

| ID | Requerimiento | Evidencia que lo demostraría | Prioridad |
|---|---|---|---|
| RF-01 | Adquirir pH desde módulo AFEL conectado a ADC1, con adaptación segura | Datos ADC + buffer de calibración + esquema | Crítica |
| RF-02 | Obtener temperatura por DS18B20 | Serie °C + termómetro de referencia | Alta |
| RF-03 | Aplicar decisiones BLUE/GREEN/YELLOW/RED/MAGENTA coherentes y con histéresis | Pruebas de FSM y secuencia real | Crítica |
| RF-04 | Mostrar el mismo estado en LED y web | Video/log con marcas de tiempo y JSON | Crítica |
| RF-05 | Crear red WaterAlpha sin Internet | Conectar segundo equipo al SoftAP + abrir IP real | Crítica |
| RF-06 | Publicar contrato JSON v1 válido | Respuesta `/api/readings` y aserciones | Alta |
| RNF-01 | Evitar falsas afirmaciones de potabilidad | Textos, guion y pantalla revisados | Crítica |
| RNF-02 | Impedir voltajes inseguros en entradas | Medición con multímetro; documentación de adaptación | Crítica |
| RNF-03 | Seguir funcionando frente a desconexión de sensor | Estado MAGENTA y log sin falsa lectura | Crítica |
| RNF-04 | Mantener servicio integral por ≥5 min (Avance #2) | Registro de prueba cronometrada | Crítica |
| RNF-05 | Cámara húmeda aislada de electrónica | Inspección CAD, montaje y prueba de estanqueidad proporcionada | Alta |
| RNF-06 | Reproducibilidad del entorno | Commit, versiones, instalación y script de tests | Alta |

## 5. Arquitectura técnica auditable

```text
MUESTRA DE AGUA
    ├─ DS18B20 (3,3 V, 1-Wire, pull-up ~4,7 kΩ) ─┐
    └─ electrodo pH → módulo AFEL (~5 V, por confirmar)
                          → protección/adaptación → ADC1 del ESP32
                                                     ↓
        adquisición + calibración verificada + estabilidad + validación
                                                     ↓
                    WAController + WA::StateMachine
                                                     ↓
                          Snapshot / decisión única
                             ├─ LED RGB PWM (pines pendientes)
                             └─ WebServer HTTP :80 /api/readings
                                      ↓
                                Wi-Fi SoftAP local
                                      ↓
                                Web WaterAlpha
```

En simulación, `WAMock::read(elapsedMs)` reemplaza las adquisiciones físicas y genera pH/T/diagnósticos ficticios. El servidor Python (`tests/mock_http_server.py`) es un **doble de prueba HTTP**, no ejecuta C++ ni demuestra que el ESP32 funcione.

## 6. Criterios de ingeniería y decisiones tomadas

- **Hardware previsto actualizado:** placa personalizada UAI con módulo clásico Ai-Thinker ESP-32S; no sustituir por FireBeetle sugerida en la base investigativa.
- **Sensores Alpha:** temperatura DS18B20 y pH AFEL. Conductividad y turbidez pertenecen a alcance futuro; CAD puede reservar espacio, pero no decir que hoy miden.
- **ADC1 + Wi-Fi:** planificar pH en entrada ADC1 efectivamente accesible; no asignar GPIO hasta revisar pinout UAI.
- **5 V vs 3,3 V:** pH AFEL alimentado según ficha real y salida adaptada; nunca aplicar 5 V sin comprobar a un GPIO.
- **DS18B20:** alimentación propuesta 3,3 V y pull-up ~4,7 kΩ entre DQ y 3V3; verificar colores y variante antes de cablear.
- **Máquina de estados:** límites pH 6,5–8,5 son provisionales; margen 0,1 y liberación por histéresis 0,1; no confundir con regulación sanitaria.
- **LED RGB:** cinco colores, posible ánodo/cátodo común; tres resistencias limitadoras calculadas según ficha y GPIO; cableado pendiente.
- **Alimentación/carcasa:** USB 5 V para primeras pruebas y separación húmedo/seco. IP67, masa objetivo y autonomía son hipótesis hasta ensayos.

## 7. Evaluación del método de prueba

**Logros comunicados:** visualización web local MOCK, simulación de controles, compilación Arduino, instalación MSYS2 `g++`, pruebas unitarias de estados, pruebas HTTP desde servidor Python, generación de imagen LittleFS y tests sintéticos de calibración/pipeline. El resultado literal `WaterAlpha: pruebas OK` fue confirmado expresamente para la FSM; otros casos deben acompañarse con salida de consola conservada en evidencias.

**No está demostrado aún:** medición real de pH, exactitud, estabilidad del módulo AFEL, calibración con patrones, temperatura real, consumo, autonomía, estanqueidad, conectividad SoftAP real, LED físico, ≥5 minutos de operación integrada.

**Iteraciones aprovechables para el informe:** solución de conflicto de macro Arduino `HIGH/LOW` al renombrar `Anomaly::ABOVE_MAX/BELOW_MIN`; corrección de `#include <LittleFS.h>`; tratamiento de URL IPv6 `http://[::]:8000/` mediante localhost/127.0.0.1. Documentar problema→diagnóstico→modificación→nueva compilación, pero **no presentar debugging como sustituto de iteración física**.

## 8. Hallazgos y no conformidades prioritarias

| Severidad | Hallazgo | Acción verificable |
|---|---|---|
| CRÍTICA | Sin ESP32 físico ni datos de sensores reales | Adquirir placa y montar prueba sensor→FSM→LED→HTTP |
| CRÍTICA | Señal analógica AFEL no caracterizada | Identificar referencia exacta; medir salida min/max; diseñar interfaz protectora |
| CRÍTICA | `ph_pipeline` conserva `signal_validated=false` y `calibration_verified=false` en fuente real | Diseñar protocolo y mecanismo de habilitación; nunca activar por simple cambio de dos booleanos |
| ALTA | Pinout y LED no confirmados; GPIO `-1` | Obtener esquema UAI + tabla GPIO realmente expuestos |
| ALTA | Evidencia de CAD no inspeccionada | Contrastar ensamble con dimensiones de componentes reales |
| ALTA | Imagen LittleFS construida pero no cargada ni probada en ESP32 | Validar partición, carga y rutas de archivos en placa |
| ALTA | Solo pruebas de software unitarias y dobles HTTP | Agregar integración firmware real y logs continuos |
| ALTA | Fuente y esquema electrónico incompletos | BOM, protecciones, cálculos y diagrama actualizado |
| MEDIA | Doble origen web `web/` versus copia `firmware/WaterAlpha/data/` | Automatizar sincronización y test de igualdad antes de generar FS |
| MEDIA | `sample_age_ms` no tiene política de vencimiento y no consta test desconexión Wi-Fi | Establecer TTL/estado sin conexión y ensayo de reconexión |
| MEDIA | Documentación de librerías/versión/commit dispersa | Tutorial único, `README`, lista de dependencias y evidencia de entorno |
| MEDIA | `MOCK_MODE` general versus flags independientes de sensores y `API_MODE` | Nombrar y documentar las tres capas para evitar confusión |
| MEDIA | Diferencias de mensajes JS/C++ y prioridad de fallos | Pruebas de paridad de contrato, estados y mensajes |
| MEDIA | Reporte académico no iniciado/auditado | Redactar con evidencia y anexos desde plantilla |

## 9. Auditoría de seguridad y comunicación responsable

**No dar por aprobado**: IP67, potabilidad, detección de patógenos, cumplimiento integral OMS/NCh409, precisión ±0,1 pH, autonomía o temperatura compensada sin pruebas. La investigación de base describe referencias, alternativas y argumentos, no ensayos del equipo. Evitar etiquetar `GREEN` como seguro; usar «sin anomalía detectada en pH validado bajo la referencia preliminar».

## 10. Recomendaciones ordenadas

**Fase A, sin hardware:** comprobar tests 6.7, revisar repo en commit fijo, congelar contrato JSON v1, documentar lista de dependencias, preparar PPT e informe con estado real, medir dimensiones en CAD, planificar BOM y banco de pruebas.  
**Fase B, al recibir hardware:** verificar referencia/Flash/pinout; alimentación aislada y niveles; DS18B20; salida AFEL sin conectar al ADC antes de adaptar; LED; firmware/FS; pruebas por subsistema.  
**Fase C, integración:** soluciones patrón, calibración, validación independiente, fallos, captura de logs 5 min, vídeo integral, actualización de diagramas y manuscrito.  
**Fase D, revisión académica:** cotejar 4 entregables del equipo con requerimientos actualizados y repositorio; unificar nombres de variables, unidades, versiones y limitaciones.

## 11. Puertas de aprobación (gates)

- **G0 — Software preparado:** pruebas reproducibles, Arduino compila, contrato web, LittleFS generado. **Estado: alcanzado según reportes; pendiente auditoría directa del commit.**
- **G1 — Electrical safe:** ficha y pinout UAI, mediciones de voltaje, ADC protegido, corriente LED limitada. **Pendiente/bloqueado.**
- **G2 — Sensores funcionando:** datos con fuente real identificada, errores explícitos y calibración trazable. **Pendiente/bloqueado.**
- **G3 — Prototipo Alpha integrado:** LED físico, SoftAP y web real durante ≥5 min sin fallos relevantes. **Pendiente/bloqueado.**
- **G4 — Entrega documental:** PPT, informe APA con evidencia, CAD revisado y anexos. **Pendiente.**

## 12. Fuentes de control

- `Instructivo_Entrega_2_Informe_APA.docx`: §1–9, especialmente funcionamiento real, testing, estructura, anexos y rúbrica.
- `Plantilla_Entrega_2_APA.docx`: títulos y secuencia del paper.
- `avance_02_estudiantes.pdf`: pp. 2–8, demo presencial, 5–7 min, cinco minutos estables, CAD, equipos, resultados y ponderaciones.
- `Base teórica y técnica...pdf`: pp. 1–13 y bibliografía, límites del método, alternativas y recomendaciones **preliminares**.
- `Clase_TEI201_Sistemas_Electronicos_Control.pptx`: esquema sensor/adquisición/procesamiento/actuación, tensiones, histéresis y fallos.
- `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx`: parámetros, interferencias, cotas, componentes y fabricación.
- Historial técnico conversacional y `prompt_wateralpha_software_interfaz_web(1).txt`: arquitectura y módulos implementados/descritos.

**Documentos asociados:** [02 Presentación](02_AUDITORIA_PRESENTACION_PPT_Y_DEMO.md) · [03 Prototipo](03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md) · [04 Informe](04_AUDITORIA_INFORME_ENTREGA2_APA.md) · [05 Tutorial](05_TUTORIAL_REPLICACION_ENTORNO_Y_SOFTWARE.md) · [06 Matriz](06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md) · [07 Riesgos](07_REGISTRO_RIESGOS_Y_PENDIENTES.md).
