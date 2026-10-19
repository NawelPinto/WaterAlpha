# Auditoría especializada — Informe de Entrega 2 bajo instructivo APA


## 1. Requisitos formales obligatorios

| Variable | Exigencia del instructivo |
|---|---|
| Formato final | PDF; nombre sugerido `E2_GrupoXX_NombreProyecto.pdf` |
| Extensión principal | **Máximo 10 páginas**, desde el título hasta contribuciones de autor |
| Referencias y anexos | Fuera del límite de 10 páginas |
| Página y columnas | A4, **una columna** |
| Márgenes | 2,0–2,2 cm |
| Tipografía | Times New Roman, 11 pt |
| Interlineado | 1,08–1,15; texto justificado |
| Portada | **No** usar portada independiente |
| Resumen | 150–200 palabras recomendadas; problema, solución, método, resultado, límite, siguiente paso |
| Palabras clave | 3–5 |
| Figuras/tablas | Numeradas y mencionadas; APA 7, fuente y nota cuando corresponda |
| Referencias | APA 7; verificar fuentes originales y datasheets, no citar una cifra sin sustento |
| Integridad | Declarar uso pertinente de IA, código, librerías y recursos externos en anexos |

El instructivo propone una distribución orientativa de **5 páginas** y a la vez establece un máximo de **10**. No es una contradicción: **5 es sugerencia editorial, 10 es límite**. Recomendar 6–8 páginas principales si la evidencia lo requiere, sin rellenar ni sacrificar legibilidad.

## 2. Estructura requerida y contenido específico WaterAlpha


### Resumen (150–200 palabras)

Redactar **al final** y utilizar verbos con nivel de evidencia: «se diseñó», «se implementó en C++», «se compiló», «se probó con datos sintéticos», «se ejecutó en ESP32» **solo si se hizo**. Incluir: necesidad del senderista; arquitectura pH AFEL + DS18B20 + ESP32 + LED + SoftAP; pruebas hechas; al menos un resultado cuantitativo real de pruebas; restricción principal (ausencia o validación pendiente de hardware). Tres a cinco palabras clave, por ejemplo: prototipo Alpha, ESP32, pH, sensórica, interfaz web local.

### Introducción (breve)

1–2 párrafos: agua superficial y dificultad de evaluación visual; usuario senderista, eventual aplicación secundaria SSR; alcance preliminar. Describir brecha y objetivos de Entrega 2: pasar de problema a arquitectura comprobable. Requerimientos concretos: portabilidad, dos sensores en Alpha, autónomo en conectividad, LED/web, cámara húmeda aislada, resultados no clínicos/no sanitarios. No repetir íntegramente Entrega 1.

**Cuidado de fuentes:** el estudio teórico contiene estadísticas citadas y referencias OMS, ONU, SEREMI, NCh409; antes de usarlas verificar documentos originales, año, ámbito y definiciones. No convertir referencias de pH 6,5–8,5 en «norma universal para potabilidad».

### Materiales y métodos — arquitectura del sistema

Incluir Figura 1 (diagrama propio) con muestra → electrodo pH/módulo AFEL/ADC1 y DS18B20/1-Wire → ESP32/`WAController`/FSM → RGB + HTTP `/api/readings` → celular vía SoftAP. Dibujar alimentación en carril aparte. Señalar MOCK frente a REAL según la versión evaluada. Indicar qué se ejecuta en navegador y qué en firmware; protocolo JSON `schema_version:1` y campos clave. Diferenciar «supervisión y alerta» de «control de la potabilidad»: WaterAlpha no altera el agua.

### Materiales y métodos — selección y electrónica

Tabla compacta: placa UAI Ai-Thinker ESP-32S (ESP32 clásico, 3,3 V de lógica), DS18B20 (1-Wire, 3,3 V, pull-up 4,7 kΩ propuesto), sonda y módulo AFEL (~5 V según identificación preliminar, salida por caracterizar), LED RGB (tres resistencias, común por determinar), fuente USB 5 V de prueba, componentes de adaptación, carcasa/cámara y conectores. No usar ficha del módulo pH DFRobot para reportar exactitud del modelo AFEL.

**Esquemático:** actualizado a lo efectivamente cableado; si no hay pines identificados, presentarlo como **diagrama de arquitectura eléctrica preliminar**, no como «esquemático final». Adjuntar el esquema completo al Anexo B cuando exista. Informar límites de tensión, separación húmedo/seco, GPIO por verificar y ADC1 requerido durante Wi-Fi. Mostrar pseudocódigo FSM con prioridades: error → inicialización → validación → estabilidad → umbrales/histéresis → LED y web. No copiar páginas completas de código.

### Materiales y métodos — CAD e integración

Usar Figura 2 del **modelo Fusion 360 realmente desarrollado por el grupo**; vista isométrica y de sección, cotas reales, posición UAI PCB, batería prevista, LED, paso de cable, sonda, cámara para muestra, acceso USB y mantenimiento. El estudio base sugiere un cilindro de ≈Ø80 × 175–185 mm, cámara 100–130 mL y objetivo <400 g: son **hipótesis de referencia, no especificaciones medidas del CAD actual**. Solo citarlas como propuestas descartadas/adoptadas después de comprobar el diseño real. No afirmar IP67 probado por usar juntas.

### Materiales y métodos — pruebas y metrología

Tabla 2 con columnas **ID / condición / procedimiento / variable-unidad / criterio predefinido / resultado / evidencia**. Mínimos: compilación; tests de FSM; prueba sintética de calibración/pipeline; HTTP Python; construcción de FS; pruebas pendientes de sensores/LED/SoftAP. Diferenciar `pruebas unitarias` de `integración software` y `integración física`. Para pH, reportar voltaje ADC (mV), pH (sin unidad), patrones de referencia, pendiente e intercepto **solo al medir**; para temperatura, °C y referencia independiente; reportar precisión/resolución del fabricante únicamente para modelo exacto con cita.

### Resultados

Incluir evidencia **realmente obtenida** y su interpretación:

- Captura de compilación Arduino (board package/versión y fecha); no presentarla como ejecución en placa.
- Consola de tests `WaterAlpha: pruebas OK` para FSM y otras salidas solo si están archivadas; adjuntar nombre de ejecutable/fuente y condiciones.
- Screenshot/video de web `mode:"MOCK"` consumiendo HTTP Python; decir «simulación de transporte HTTP», no «ESP32 conectado».
- Resultado de generación de `mklittlefs.bin` como empaquetado, no despliegue.
- Evidencia CAD con dimensiones verificadas.
- Al menos **una iteración documentada** como problema→cambio→efecto: error macro `HIGH`→renombrar enumeradores→compilación, o `include <LittleFS.h>` sin `#`→corrección→compilación. Para nota alta, sumar iteración física cuantificada cuando exista.
- Si no hay mediciones reales, escribir «No se efectuaron mediciones experimentales de pH/temperatura ni calibración física durante esta fase».

### Análisis y discusión

Comparar resultados con requerimientos, sin triunfalismo. Responder qué se comprobó; qué sigue pendiente; limitación ADC interno y adaptación de 5 V; necesidad de calibración térmica y buffers; conectividad Wi-Fi prevista; diferencia entre GREEN y potabilidad; posibilidad de sesgos por ruido/deriva; coherencia dimensional CAD. Describir impacto técnico y plan priorizado antes de Entrega 3.

### Conclusión

Un párrafo: logro verificable, principal iteración, límite prioritario y siguiente hito. No afirmar sensores probados si solo hubo compilación.

### Contribuciones de autor

Declarar por integrante responsabilidades **reales** (CAD, documentación, C++, electrónica, pruebas, IA/edición), y responsabilidad colectiva de revisión. No usar ejemplos de la plantilla como si fueran distribución factual del grupo.

## 3. Anexos exigidos y lista de evidencias

| Anexo | Exigencia | Qué preparar |
|---|---|---|
| A — Actas del equipo | Actas escaneadas legibles, fecha/asistentes/acuerdos/tareas/responsables | Actas del equipo reales, no reconstruidas ficticiamente |
| B — Esquemático/diagramas | Esquema completo si no cabe en texto principal | Esquema eléctrico con pines confirmados y referencias; distinguir preliminar |
| C — Código/firmware | Código completo o vínculo a repo/carpeta | URL accesible o archivo ZIP, commit y tabla de dependencias |
| D — Testing | Datos, capturas, logs, fotos, gráficos extensos | `EVID-` con fecha/condición/test/tipo de dato/resultado |
| E — IA/código externo | Herramienta/fuente, propósito, parte y adaptación | Declaración del uso de ChatGPT, bibliotecas y tutoriales reales |

Los anexos no sustituyen resultados esenciales en cuerpo principal. La plantilla contiene campos explícitos para A–E y figuras/tablas; no dejar placeholders en entrega final.

## 4. Rúbrica del instructivo del paper (literal)

| Criterio | Peso |
|---|---:|
| Funcionalidad demostrada del prototipo Alpha | **25 %** |
| Integración sensor–procesamiento–control/actuador–conectividad | **20 %** |
| Diseño electrónico, esquemáticos y justificación | **15 %** |
| CAD preliminar e integración física | **10 %** |
| Testing, metrología e iteración | **15 %** |
| Resultados, análisis y plan de mejora | **10 %** |
| Calidad paper APA, figuras, referencias y anexos | **5 %** |

La suma es 100 %. **No confundir** con la rúbrica de `avance_02_estudiantes.pdf`, que corresponde a otra pauta.

## 5. Distribución editorial recomendada

| Sección | Extensión tentativa dentro del cuerpo |
|---|---:|
| Título, autores, resumen y palabras clave | ~0,5 p. |
| Introducción | ~0,7 p. |
| Materiales y métodos, arquitectura, electrónico y CAD | ~2,0 p. |
| Protocolo y resultados | ~1,5 p. |
| Discusión | ~1,0 p. |
| Conclusión y contribuciones | ~0,5 p. |
| **Total orientativo** | **~6,2 páginas**, ajustable ≤10 |

No usar «introducción muy larga + resultados nominales»; desplazar espacio hacia evidencia, metrología y análisis.

## 6. Auditoría de afirmaciones y lenguaje científico

| Afirmación riesgosa | Reformulación correcta con situación actual |
|---|---|
| «El sensor mide pH ±0,1» | «El firmware prepara ADC y calibración; exactitud del AFEL no validada» |
| «Creamos una red Wi-Fi y funciona en celular» | «El firmware SoftAP compila; prueba física pendiente» |
| «La web muestra mediciones reales» | «La web muestra respuestas JSON simuladas desde Python / navegador» |
| «Prototipo estanco IP67» | «CAD contempla separación húmedo/seco; grado IP pendiente de ensayo» |
| «Agua segura si LED verde» | «No se detectaron anomalías en los parámetros medidos bajo referencias preliminares» |
| «Implementamos cuatro sensores» | «Alpha implementa/prepara pH y temperatura; CE/NTU son futuras ampliaciones» |

## 7. Plantilla mínima de registro de resultado

```text
EVID-ID: EVID-XX
Fecha / hora: [POR COMPLETAR]
Commit: [HASH DEL REPOSITORIO]
Prueba: [ID + objetivo]
Montaje/ambiente: [PC o ESP32 físico; sensores y fuente]
Variable y unidades: [pH; °C; mV; tiempo]
Condición de referencia: [patrón/termómetro/condición MOCK]
Criterio de aprobación: [establecido antes]
Resultado observado: [valor/estado + dato original]
Evidencia: [archivo real/log/fotografía]
Limitación: [si existe]
Decisión: APROBADO / FALLIDO / PENDIENTE
```

## 8. Checklist editorial final

- [ ] El título refleja el sistema efectivamente probado.
- [ ] 150–200 palabras de resumen, 3–5 palabras clave.
- [ ] A4, una columna, márgenes 2,0–2,2 cm, TNR 11, 1,08–1,15, justificado.
- [ ] Sin portada separada; texto principal ≤10 páginas.
- [ ] Secciones de plantilla completas y sin corchetes-placeholder.
- [ ] Figura de bloques; tabla de componentes; figura CAD real; pruebas y resultados interpretados.
- [ ] Esquemático actualizado, pines/voltajes solo confirmados.
- [ ] Unidades, datos medidos, referencias y límites metrológicos expresos.
- [ ] La discusión distingue implementado, compilado, simulado y probado físicamente.
- [ ] Registro de una iteración real con antes/después verificable.
- [ ] Contribuciones de autor basadas en trabajo real.
- [ ] Figuras/tablas APA7 con fuente y mención en texto; bibliografía verificada.
- [ ] Anexos A–E incluidos según su condición y fuente.
- [ ] Declaración de IA, código externo y bibliotecas.
- [ ] PDF final abierto y revisado visualmente por el equipo.

