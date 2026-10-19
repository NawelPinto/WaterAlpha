# Matriz de trazabilidad — Requisitos, evidencias y cuatro entregables

**WaterAlpha · TEI201 UAI · Corte 18-10-2026**  
**Propósito:** transformar las auditorías en una lista verificable de evidencia para PPT, informe, CAD y demo web.  
**Unidad de evidencia:** archivo auténtico con fecha, condición y `commit`.  
**Niveles:** V-reportado = usuario informó prueba, no hubo revisión independiente del repositorio; D = desarrollo descrito; P = pendiente; B = bloqueado por hardware/datos.

## 1. Cruce de fuentes oficiales y entregables del equipo

| Fuente | Requisito documentado | Entregable afectado | Evidencia requerida |
|---|---|---|---|
| `Instructivo_Entrega_2_Informe_APA.docx`, §§1–3 | Arquitectura, componentes, esquemático, CAD, testing, resultados, discusión | Informe y también PPT/demo | Figuras 1–3, tabla de componentes, metodología, resultados interpretados |
| Mismo instructivo, §2 | A4, 1 columna, ≤10 págs, TNR 11, APA, PDF, sin portada | Informe | PDF final inspeccionado |
| Mismo instructivo, §§4–6 | APA7, referencias, autorías, anexos A–E, declaración IA/código | Informe | Referencias originales, actas, código, evidencias, IA |
| Mismo instructivo, §8 | Pondera funcionalidad 25 %, integración 20 %, etc. | Informe | Matriz de resultados respecto de rúbrica |
| `Plantilla_Entrega_2_APA.docx` | Estructura obligatoria concreta | Informe | Documento sin placeholders y anexos completos |
| `avance_02_estudiantes.pdf`, pp. 2–4 | Demo sensor-actuador-conectividad, CAD, pruebas/iteración | PPT/demo y prototipo | Video y log real; pantallas y montaje; CAD |
| `avance_02_estudiantes.pdf`, pp. 2 y 5–8 | 5–7 min, todos participan, ≥5 min estable, preparación y plan B | PPT/demo | Reloj/log, guion, asignación de roles, plan B |
| `Clase_TEI201_Sistemas_Electronicos_Control.pptx` | Arquitectura, voltajes, adquisición/driver/actuación, fallos | Prototipo e informe | Diagrama eléctrico, pruebas de adquisición y actuador |
| `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx` | Modelo paramétrico, dimensiones, interferencias, fabricación | CAD e informe | Archivo Fusion editable, vistas, cotas, decisiones |
| Base teórica/técnica PDF, pp. 1–13 | Arquitectura y límites de interpretación del agua | Informe, PPT, prototipo | Alcance definido; referencias verificadas antes de citar |
| Definición del **equipo** (esta conversación) | Cuatro productos: Fusion, PPT, informe, demostración website | Todos | Carpeta final con cuatro elementos y evidencias |

**Cautela:** la pauta PDF de Avance #2 (fecha 12-10-2026) y la Entrega 2 paper pueden ser instancias distintas. La exigencia de PPT procede de la definición del equipo; la cantidad 5–6 slides es **recomendación**, no requisito explícito encontrado.

## 2. Matriz de requisitos trazables y pruebas

| ID | Requisito | Archivo/código relacionado | Prueba / condición | Evidencia por adjuntar | Estado |
|---|---|---|---|---|---|
| RQ-01 | Interfaz usable sin Internet en producto final | `web/*`, `data/*` | HTTP en ESP32 SoftAP real | `EVID-HW-01` | B |
| RQ-02 | LED y pantalla reproducen estado único | `controller.*`, `led_colors.*`, `led_rgb.*`, `app.js` | Comparar estados simultáneos | `EVID-HW-04` | B |
| RQ-03 | FSM cinco estados e histéresis | `state_machine.*`, `types.h` | `test_state_machine.exe` | `EVID-SW-01` | V-reportado |
| RQ-04 | Temp °C disponible | `temperature_sensor.*`, `OneWire`, `DallasTemperature` | DS18B20 vs termómetro | `EVID-HW-02` | B |
| RQ-05 | pH adquirido en ADC1 seguro | `ph_sensor.*`, `pins.h`, interfaz física | Multímetro y mediciones mV | `EVID-HW-03a` | B |
| RQ-06 | pH real calibrado y verificable | `ph_calibration.*`, `ph_pipeline.*` | Patrones + verificación independiente | `EVID-HW-03b` | B |
| RQ-07 | Nunca presentar medida no válida como buena | `ph_pipeline.*`, FSM, web | Sensor desconectado, no calibrado → MAGENTA | `EVID-HW-05` | D/B |
| RQ-08 | API JSON v1 interoperable | `api.js`, `web_server.cpp` | Python HTTP 8001 + Arduino real luego | `EVID-HTTP-01` y `EVID-HW-01` | V-reportado/B |
| RQ-09 | Código ESP32 compilable | `WaterAlpha.ino`, `.h`, `.cpp` | Arduino IDE board target correcto | `EVID-SW-06` | D |
| RQ-10 | Imagen de UI empaquetada en Flash | `data/`, LittleFS | Build + Upload en ESP32 | `EVID-FS-01` / `EVID-HW-01` | V-reportado/B |
| RQ-11 | CAD acomoda componentes y servicio | Archivo Fusion del equipo | Medición, vista de sección, revisión de interferencia | `EVID-CAD-01` | D |
| RQ-12 | Esquemático reproduce montaje vigente | `pins.h`, diagrama externo | Inspección de conexiones / BOM | `EVID-EL-01` | B |
| RQ-13 | Ensamble sin agua en electrónica | CAD y prototipo | Ensayo de manipulación/estanqueidad acordado | `EVID-MEC-01` | B |
| RQ-14 | Demostración integral ≥5 min | Firmware+web+sensores+LED | Cronometría y registro ininterrumpido | `EVID-DEMO-01` | B |
| RQ-15 | Contenido APA del paper | Word/PDF Entrega 2 | Checklist de instructivo + revisión | `EVID-DOC-01` | P |
| RQ-16 | PPT breve y participativo | PPT + guion | Ensayo, tiempos, roles | `EVID-PPT-01` | P |
| RQ-17 | Se puede replicar en segundo equipo | GitHub + tutorial 05 | Clonado y tests desde cero | `EVID-REP-01` | D |
| RQ-18 | Iteración documentada y cuantificada | historial Git, logs, notas | Antes/después y causa/solución | `EVID-IT-01` | D |
| RQ-19 | Transparencia IA y bibliotecas | anexos E + `docs/` | Declaración y fuentes | `EVID-DOC-02` | P |

## 3. Evidencia de software — registrar sin inventar resultados

| Código evidencia | Prueba y programa | Condición previa | Resultado reportado | Archivo esperado / acción |
|---|---|---|---|---|
| EVID-SW-01 | FSM `test_state_machine.exe` | `g++` MSYS2 UCRT64 | «WaterAlpha: pruebas OK» comunicado expresamente | Captura/log con commit y versión de compilador |
| EVID-SW-02 | `test_ph_calibration.exe` | calibración sintética | Desarrollado; no se inspeccionó output | Guardar 12 aserciones reales de consola |
| EVID-SW-03 | `test_ph_pipeline.exe` | evidencia sintética | Desarrollado; sin log auditado | Guardar 5 aserciones reales |
| EVID-SW-04 | `test_system_flow.exe` | 12 escenarios cronológicos | Usuario indicó «todo ok» en etapa | Guardar 17 comprobaciones con salida real |
| EVID-SW-05 | `test_led_colors.exe` | mapa 5 estados | No existe salida explícita final en contexto | Ejecutar, guardar 5 pruebas |
| EVID-SW-06 | Compilación Arduino firmware | Arduino core ESP32 + OneWire/DallasTemperature | Compilación reportada en distintas etapas; 6.7 por confirmar | Captura final con core, selección de placa y commit |
| EVID-HTTP-01 | Página de servidor Python HTTP | `data/config.js` API real + endpoint 8001 | Usuario comunicó éxito | Video + screenshot JSON donde se vea `mode:MOCK` |
| EVID-FS-01 | Imagen LittleFS | complemento `arduino-littlefs-upload` | Imagen creada, confirmado expresamente | Captura nombre/tamaño y esquema provisional |

**Prohibición editorial:** no convertir una prueba que valida **formato de JSON** en validación de **exactitud de sensor**, ni una prueba sintética de calibración en calibración de campo.

## 4. Matriz de ilustraciones y anexos del informe

| Elemento | Qué debe aparecer | Estado |
|---|---|---|
| Figura 1 — Arquitectura Alpha | DS18B20, AFEL, acondicionamiento, ESP32, LED RGB, SoftAP/web; bloques reales/simulados diferenciados | D; diagramar limpio |
| Tabla 1 — Selección de componentes | Especificaciones del modelo **real**, fuente bibliográfica y decisión | P; AFEL/UAI por identificar |
| Figura 2 — CAD Fusion 360 | Vista real de ensamble, dimensiones, acceso, aislamiento, sondas | D; extraer del CAD y acotar |
| Tabla 2 — Protocolo/resultados | ID, condición, criterio, resultado observado, evidencia | D; faltan ensayos físicos |
| Figura 3 — Evidencia representativa | Screenshot/serie/log con unidades y condición, nunca captura aislada | P; seleccionar prueba final |
| Anexo A | Actas fechadas del equipo | P |
| Anexo B | Esquema completo revisado | B |
| Anexo C | Código completo o vínculo repo más commit | D; registrar enlace/hashes |
| Anexo D | Datos/logs/fotos/testing | D; recopilar y clasificar |
| Anexo E | IA, código y recursos externos declarados | P |

## 5. Estructura de carpetas de evidencias sugerida

```text
docs/
├── 00_LEEME_AUDITORIAS.md
├── 01_AUDITORIA_GENERAL_WATERALPHA.md
├── ... [resto de auditorías]
├── 06_mapa_electronico.md        # documento previo del equipo
├── 07_protocolo_pruebas.md       # documento previo del equipo
└── evidencias/                  # crear si el equipo lo considera apropiado
    ├── sw/                      # consola tests, core, commit
    ├── http/                    # capturas API con modo MOCK visible
    ├── cad/                     # renders reales Fusion y cotas
    ├── hardware/                # fotos ensayos, multímetro y pinout
    ├── metrologia/              # patrones y series originales
    ├── demo/                    # registros >=5 minutos
    └── informe/                 # manuscrito exportado y controles
```

El material pesado puede mantenerse fuera de Git o en almacenamiento autorizado; conservar **identificador, ubicación y responsable** dentro de esta matriz. Nunca borrar datos brutos al resumirlos para el paper.

## 6. Ficha para completar cada evidencia

```markdown
### EVID-[TIPO]-[NN]
- Fecha y hora:
- Responsable:
- Commit Git (hash completo o abreviado sin ambigüedad):
- Equipo / OS / versión core:
- Código de prueba / parámetro:
- Condición (MOCK / ESP32 físico / sensor físico):
- Criterio establecido ANTES de la prueba:
- Resultado numérico y unidades:
- Archivo auténtico: [ruta o enlace]
- Repeticiones y estabilidad:
- Fallas/anomalías:
- Conclusión permitida y limitaciones:
- Estado: PENDIENTE / APROBADO / FALLIDO
```

## 7. Revisión de coherencia entre entregables

**Una sola versión de verdad:** identificar un `commit`, una lista BOM, un esquema, una versión CAD y una tabla de pruebas. El PPT, el paper y la demo deben describir **la misma arquitectura y el mismo estado de implementación**.

**Validaciones cruzadas obligatorias:**

- Si PPT dice que hay dos sensores físicos, el paper debe mostrar evidencia de ambos; si solo hay MOCK, rotular ambos como simulados.
- Si el paper dice «Wi-Fi offline probado», aportar captura celular conectado a la red del ESP32, no únicamente localhost.
- Si el CAD incluye una placa distinta a UAI ESP-32S, actualizar CAD o declarar que es volumetría sustituta con dimensiones pendientes.
- Si se afirma resolución, rango o exactitud, citar la ficha **del módulo exacto**, no de un ejemplo del PDF teórico.
- Si aparece GREEN, conservar siempre la advertencia de potabilidad y condiciones de validez.
- Si se menciona una iteración, encontrar sus commits/logs **antes y después**.

## 8. Responsables y fecha de revisión

| Entregable | Responsable | Próxima evidencia crítica | Fecha compromiso |
|---|---|---|---|
| CAD Fusion | [POR COMPLETAR] | vistas, cotas, interferencias | [POR COMPLETAR] |
| PPT/demo | [POR COMPLETAR] | guion 5–7 min y prueba ≥5 min real | [POR COMPLETAR] |
| Informe APA | [POR COMPLETAR] | primera versión con Resultados reales | [POR COMPLETAR] |
| Hardware/software | [POR COMPLETAR] | pinout, BOM, calibración y prueba end-to-end | [POR COMPLETAR] |
| Tutorial replicación | [POR COMPLETAR] | video de instalación en segunda PC y test | [POR COMPLETAR] |

## 9. Fuentes

Ver tabla inicial; detalles de formato académico en [04_AUDITORIA_INFORME_ENTREGA2_APA.md](04_AUDITORIA_INFORME_ENTREGA2_APA.md) y verificaciones técnicas en [03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md](03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md).
