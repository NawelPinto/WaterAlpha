# Auditoría y plan de integración — WaterAlpha Avance 3

**Corte:** 16 de noviembre de 2026. **Clasificación:** `VERIFICADO` significa leído/probado directamente; `REPORTADO` comunicación del usuario; `PENDIENTE` falta evidencia; `BLOQUEADO` falta pieza/acceso.

## Estado de los entregables

| Item | Estado | Observación |
|---|---|---|
| C++ controlador/estados/calibración | VERIFICADO EN VOLCADO TXT | Existe estructura, pero no se dispone aquí de un checkout real GitHub ni logs actuales de compilación Arduino |
| Dashboard web | PROPUESTA IMPLEMENTADA | El volcado TXT no incluye el código original `web/*.js/html/css`; este paquete entrega un frontend nuevo compatible con API schema_version=1 |
| `mode` de API | BUG IDENTIFICADO Y CORREGIDO EN PARCHE | Original decía `MOCK` siempre, incluso con adquisición REAL |
| Pines `25`, `34` y `PINOUT_CONFIRMED=true` | RIESGO IDENTIFICADO | No existe pinout UAI que avale esos valores. Parche los reinicia a -1 y false |
| Medición de pH en hardware | BLOQUEADO | Requiere medición AO, adaptación eléctrica, patrón y verificación independiente |
| Temperatura hardware DS18B20 | PENDIENTE | Configurables GPIO, librería, 4,7 kΩ y comparación con referencia |
| LED físicos RGB previos | DESACTUALIZADO | Usuario reporta **tres LED discretos**. Nuevo mapeo con parpadeos requiere resistencias/GPIO |
| Registro persistente Python | VERIFICADO POR TESTS LOCALES | CSV/JSONL probados contra servidor HTTP de test; en ESP32 real aún pendiente |
| Enclosure real impreso | REPORTADO EN FOTOS | Cilindro parcial con abertura lateral y perforaciones; no estanqueidad acreditada |
| CAD completo `.f3d`/planos/renders | PENDIENTE DE ARCHIVOS REALES | No hay archivo de Fusion 360 en los adjuntos de esta solicitud; no inventar un ensamble |
| `FUENTES.md` | REPORTADO POR USUARIO | Ya existe material de fuentes; copiar/renombrar a nombre exacto exigido, confirmar declaraciones IA |
| Demo integrada ≥300 s | PENDIENTE | Hacer con sensores reales si alcanzan condiciones de seguridad/calibración; de lo contrario declarar límites |

## Priorización para el Avance 3 (trabajo secuencial)

### Prioridad 0 — Seguridad sin energizar sensores

1. Verificar la placa UAI por ambas caras e identificar conectores etiquetados `GND`, `3V3`, `5V`, GPIO ADC1 y digitales libres. Conseguir el esquema/pinout del fabricante o laboratorio UAI.
2. Determinar si LEDs son tres componentes de dos terminales o un módulo de tres cables; aquí se asume **tres LED discretos**. Comprobar resistencias **individuales** de 220–470 Ω (330 Ω candidato), no usar 10 Ω o 22,5 Ω como protección sin cálculo.
3. Medir con multímetro resistencias declaradas (¿kΩ?), voltajes de alimentación y polaridades. Identificar AO del AFEL y medir sin GPIO. Confirmar que el divisor es seguro antes de conectarlo.
4. Confirmar si los 8 cables Dupont alcanzan y preparar adicionales. Alejar muestra de protoboard/USB.

**Si faltan multímetro, resistencias LED o pinout, no improvisar cableado.** Continúa la preparación documental, web y prueba de Arduino MOCK para demostrar lo implementado sin arriesgar placa.

### Prioridad 1 — Software y artefactos demostrables sin riesgo

1. Crear rama Git `avance3-integracion` o respaldar cambios con un commit; copia de `web/` y `firmware/` preexistentes.
2. Instalar parche por carpetas revisando `git diff`. `web/` anterior no vino en el volcado; la nueva interfaz es reemplazo propuesto, no una edición exacta de los archivos del repo remoto.
3. Repetir tests unitarios preexistentes, más los tests nuevos del logger, FSM y mapeo de LEDs.
4. Compilar `.ino` con bibliotecas `OneWire` y `DallasTemperature` en Arduino IDE (la prueba local de `g++` NO sustituye esto).
5. Probar web en MOCK con controles y mensajes explícitos, sin potabilidad.
6. Copiar frontend a LittleFS con `python scripts/sync_web.py`, generar imagen y verificar esquema/Flash antes de cargarla en dispositivo.
7. Confirmar el formato `FUENTES.md` y README. Añadir `hardware/`, `diseño-3d/`, `testing/` y documentación técnica con archivos **reales**.

### Prioridad 2 — Integración escalonada en placa

1. Subir firmware MOCK a UAI, comprobar SoftAP y dashboard (datos MOCK evidentes en sitio y JSON); obtener fotos/logs.
2. Probar DS18B20 real por sí solo (modo global REAL, pH deshabilitado → MAGENTA esperado).
3. Caracterizar AO/ADC AFEL sin habilitar pH válido; registrar mV y rango real.
4. Conseguir buffers, calibrar dos puntos y verificar en un tercero. Solo después de cumplir criterio, completar `ph_field_data.h`, bandera de integridad y aprobar `GREEN` con datos reales.
5. Si resuelta la resistencia por canal, conectar 3 LED y verificar estados de salida. Para primera demo con sensores MOCK, rotularlo explícitamente.
6. Abrir el sitio y registrar con Python durante 300 s; conservar evidencia, fotos de tres ángulos y hora PC.

## Hallazgos técnicos que no deben omitirse

1. **Estado de datos inconsistente:** `config.h` de volc. indicaba `MOCK_MODE=false`, `USE_REAL_PH=true`, `USE_REAL_TEMPERATURE=false` y `pins.h` confirmado. En tal situación el `controller` podía buscar pH real sin temperatura; la FSM debía indicar error; API además reportaba erróneamente `mode:MOCK`. El paquete devuelve valores seguros por defecto.
2. **No había criterio temporal de temperatura en REAL:** el `stable` original solo verificaba amplitud de mV de pH. El parche introduce una ventana térmica de 5 lecturas, margen 0,5 °C preliminar.
3. **No hay norma simple de potabilidad basada en pH/T:** evitar “agua segura”, “apta para beber”, “potable”. Incluso `GREEN` significa solo éxito limitado del dispositivo bajo referencias fijadas.
4. **Semáforo físico:** RGB de cuatro patillas y tres LED independientes no son intercambiables. El parche adapta la salida a tres LED por GPIO; los estados lógicos siguen siendo cinco, diferenciados por parpadeo.
5. **Persistencia:** la web del ESP32 es lectura HTTP; un navegador no incorpora automáticamente mediciones a GitHub. El script Python registra al disco local, y Git se sincroniza después por red.
6. **Carcasa:** geometría abierta apta para revisión volumétrica, pero sin protección al agua demostrada y sin volumen efectivo/encaje UAI verificado; gestionar separación húmedo/seco.
7. **Calibración:** el archivo `ph_field_data.h` del código original contiene `-1` y tres banderas `false`; conservarlas hasta obtener mediciones, incluso si produce `MAGENTA` en demostración. Es mejor que simular pH real.

## Riesgos específicos y mitigaciones

| Riesgo | Severidad | Mitigación |
|---|---|---|
| Sobretensión AFEL en ADC1 | Crítica | Medir salida; dividir/acondicionar; medir nivel final antes de GPIO |
| Pines 25/34 no disponibles en placa UAI | Alta | Esquemático real; etiquetas GPIO disponibles; deshabilitar flags hasta confirmar |
| LED sin resistencia limitadora | Crítica | Conseguir 3 resistencias, confirmar Vf/corriente, GPIO y tierra |
| Sin patrón pH | Alta | Documentar mV como señal cruda; no habilitar `calibration_verified` |
| 8 cables Dupont insuficientes | Media | Inventario de conexiones, comprar/solicitar cables y puentes extras |
| Agua ingresa a electrónica | Crítica | Recipiente independiente, aislamiento, sellos; no atribuir IP sin prueba |
| Wi-Fi SoftAP desconecta Internet PC | Media | Registrar CSV offline y hacer `git push` después de volver a Internet |
| Sitio y firmware divergen | Alta | Ejecutar `sync_web.py` y subir LittleFS además del sketch |
| Solo 3 LEDs para 5 estados | Media | Patrón de parpadeo BLUE/MAGENTA + leyenda; verificar video físico |
| Exposición exagera alcance | Alta | Aviso permanente en web, README, PPT y defensa oral |

**Puerta de cierre:** el usuario deberá copiar este overlay a la clonación real y volver a compilar el firmware. Este paquete NO fue subido a GitHub ni ejecutado sobre la placa UAI; los archivos y pruebas creados localmente no constituyen evidencia física.