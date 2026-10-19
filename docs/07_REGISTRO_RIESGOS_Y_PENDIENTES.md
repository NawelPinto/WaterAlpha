# Registro de riesgos, pendientes y plan de liberación — WaterAlpha

**Universidad Adolfo Ibáñez · TEI201 · Fecha base 18-10-2026**  
**Enfoque:** la evidencia manda: ante falta de medición, declarar pendiente; no fabricar un «APROBADO».  
**Usar junto con:** [01 General](01_AUDITORIA_GENERAL_WATERALPHA.md), [03 Hardware](03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md), [06 Evidencias](06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md).

## 1. Escala usada

- **Crítico:** afecta seguridad eléctrica, confiabilidad del veredicto, cumplimiento de demostración o integridad académica; impide afirmar funcionamiento real.
- **Alto:** impide fabricar/replicar/evaluar un subsistema; debe resolverse antes de la demostración final.
- **Medio:** puede causar fallos de mantenimiento, inconsistencia o debilidad de evidencia.
- **Bajo:** mejora editorial/no bloqueante, sujeta a prioridades.

Estados: **ABIERTO, EN CURSO, BLOQUEADO, RESUELTO CON EVIDENCIA**. No marcar resuelto por haber escrito código o planeado un ensayo.

## 2. Riesgos y acciones correctivas

| ID | Nivel | Riesgo/causa | Mitigación / criterio de cierre | Dependencia / estado |
|---|---|---|---|---|
| R-01 | **CRÍTICO** | Ausencia de placa UAI, sin ejecución real | Conseguir placa; confirmar pinout/revisión; grabar firmware y FS; log físico | Hardware; BLOQUEADO |
| R-02 | **CRÍTICO** | Módulo AFEL de salida desconocida puede dañar ADC | Identificar modelo, medir tensión/polaridad/rango, acondicionar y verificar voltaje ADC | Sensor+multímetro; BLOQUEADO |
| R-03 | **CRÍTICO** | pH físico no calibrado; inferencia de potabilidad injustificada | Buffers, regresión, punto independiente, error documentado; estado MAGENTA hasta aceptación | Sensor+patrones; BLOQUEADO |
| R-04 | **CRÍTICO** | Pauta Avance #2 exige sensor–actuador–conectividad real y ≥5 min | Log/video de demo integral real; sin sustitución silenciosa por Python | Hardware; BLOQUEADO |
| R-05 | **CRÍTICO** | False GREEN (agua etiquetada como segura) | Revisar UI/PPT/paper/LED; aviso de evaluación preliminar siempre visible | Revisión comunicacional; ABIERTO |
| R-06 | **ALTO** | CAD declarado listo, pero no se vieron sus dimensiones ni BOM | Revisar Fusion nativo, interferencias, sección, acceso a BNC, masa y cierre | CAD+componentes; EN CURSO |
| R-07 | **ALTO** | GPIO UAI y tipo LED no identificados | Esquemático UAI, tabla pines, selección PWM apta, resistencias/driver, pruebas | Placa+LED; BLOQUEADO |
| R-08 | **ALTO** | Falta estabilidad/rango de señal pH en lectura real | Definir filtrado, ventana y criterio de estabilidad; verificar ruido y outliers | Hardware; BLOQUEADO |
| R-09 | **ALTO** | Flags de evidencias pH hardcoded false y calibración no persistente | Flujo de calibración, NVS/versionado, carga y revalidación; test reset/rearranque | Diseño de firmware+hardware; ABIERTO |
| R-10 | **ALTO** | LittleFS compilado pero no subido a placa | Confirmar capacidad Flash, partición, upload y GET rutas reales | ESP32; BLOQUEADO |
| R-11 | **ALTO** | Informe y PPT todavía no verificados | Crear material, ensayar, aplicar rúbricas, revisiones cruzadas, export final | Equipo; ABIERTO |
| R-12 | **ALTO** | No existen datos metrológicos para justificar precisión | Diseñar criterio previo, referencia independiente, repeticiones e incertidumbre | Laboratorio; BLOQUEADO |
| R-13 | **MEDIO** | Doble carpeta `web/` y `data/` divergen | Script/validación de copia y sincronización; comparar hashes; regenerar FS | Software; ABIERTO |
| R-14 | **MEDIO** | `sample_age_ms` no desencadena falla por dato viejo | Definir timeout de muestra y pruebas desconexión, no usar valor viejo como actual | Software; ABIERTO |
| R-15 | **MEDIO** | `state_machine.js` y C++ pueden divergir | Suite compartida de casos límite y comparación de contrato | Software; ABIERTO |
| R-16 | **MEDIO** | Los tests fueron reportados pero no existe evidencia de todas las salidas | Guardar consola, código, compilador, commit y esperados | Equipo; EN CURSO |
| R-17 | **MEDIO** | Versión/instalación poco reproducible en otra PC | Seguir tutorial 05 desde clone limpio con capturas; registrar versiones | Equipo; ABIERTO |
| R-18 | **MEDIO** | Magenta parpadea en web pero físico puede ser fijo | Decidir requisito, implementar temporización no bloqueante y verificar paridad | LED hardware; ABIERTO |
| R-19 | **MEDIO** | Sonda pH no responde y ADC aun entrega número plausible | Diseñar diagnóstico con mediciones controladas, evitar `acquired == connected` | Laboratorio; BLOQUEADO |
| R-20 | **MEDIO** | Prototipo mojado, USB o conectores expuestos | Separación húmedo/seco, juntas/pasamuros, pruebas seguridad y mantenimiento | Mecánica; BLOQUEADO |
| R-21 | **MEDIO** | Fecha/pauta de Avance #2 diferente de Entrega 2 | Confirmar horario, forma de PPT y criterios vigentes con docente | Docente; ABIERTO |
| R-22 | **BAJO** | Lenguaje y logs parciales sin acentos / estilos inconsistentes | Normalizar mensajes y versiones, mantener JSON válido | Software editorial; ABIERTO |

## 3. Dependencias que bloquean las pruebas físicas

**Cadena crítica (no alterar orden):**

```text
Recibir placa UAI y sensores reales
  → confirmar modelos, alimentación, GPIO y ficha AFEL
  → verificar tensión de fuente y salida de pH sin conectar al ADC
  → diseñar interfaz ADC + resistencias LED + pull-up DS18B20
  → compilar/subir firmware y cargar LittleFS en placa
  → probar SoftAP/web con datos MOCK
  → probar DS18B20 REAL con referencia
  → medir AFEL en mV y calibrar con patrones
  → verificar estabilidad / fallo y validación física
  → activar LED físico y comprobar sincronía
  → probar sistema integrado >=5 min
  → respaldar evidencia → actualizar CAD/PPT/informe
```

**Puntos que sí se pueden completar ahora:** slides de presentación, auditoría del CAD, modelo del circuito provisional, lista de compra, criterios de prueba, vídeo de instalación de software en otro computador, freeze de commit, evidencias de tests y documentación APA de resultados sintéticos.

## 4. Próximo sprint sin hardware — decisiones y entregables

| Prioridad | Acción | Evidencia de cierre | Responsable | Fecha |
|---|---|---|---|---|
| 1 | Verificar última compilación Arduino tras Bloque 6.7 + 5 tests LED | Captura salida/versión/commit | [POR COMPLETAR] | [POR COMPLETAR] |
| 2 | Clonar en otra PC o entorno limpio y seguir tutorial 05 | Video + hashes y log tests | [POR COMPLETAR] | [POR COMPLETAR] |
| 3 | Medir en Fusion cotas y volumen útil del CAD propio | Capturas dimensiones y sección con fecha | [POR COMPLETAR] | [POR COMPLETAR] |
| 4 | Contactar responsable técnico de placa UAI para conseguir pinout | Foto/pinmap/esquema oficial y revisión de placa | [POR COMPLETAR] | [POR COMPLETAR] |
| 5 | Conseguir referencia exacta/ficha del AFEL y DS18B20 | Datasheets guardados y fotos/modelo | [POR COMPLETAR] | [POR COMPLETAR] |
| 6 | Completar BOM con costos locales y equipo de calibración | Lista cantidades, proveedores y disponibilidad | [POR COMPLETAR] | [POR COMPLETAR] |
| 7 | Preparar PPT 5–6 láminas y guion de demo honesto | Primer PPT y tiempos de ensayo | [POR COMPLETAR] | [POR COMPLETAR] |
| 8 | Estructurar manuscrito con datos actuales y límites | Primera versión siguiendo plantilla APA | [POR COMPLETAR] | [POR COMPLETAR] |
| 9 | Confirmar con docente PPT/plazo y aceptación de simulación temporal | Correo/Teams confirmado, no suposición | [POR COMPLETAR] | [POR COMPLETAR] |

## 5. Criterios de liberación por hito

### Liberación L0: paquete documental (sin placa)

- [ ] Índice de auditorías integrado a `docs/` en un commit.
- [ ] Tests ejecutados y salidas guardadas desde clonación limpia.
- [ ] Código compilado con versión ESP32 core documentada.
- [ ] Imagen LittleFS creada sin errores y `data/` sincronizado con fuente.
- [ ] Versiones de software/librerías capturadas.
- [ ] PPT e informe expresan «preparado/probado en simulación», no «prototipo físico validado».

### Liberación L1: banco electrónico seguro

- [ ] Modelos, fuentes y niveles confirmados.
- [ ] Pinout UAI validado, ADC1 elegido correctamente.
- [ ] Divisor/adaptación AFEL revisada, salida dentro de límites.
- [ ] Resistencias LED calculadas y fuente suficiente.
- [ ] Circuito ensayado sin agua cerca de electrónica abierta.

### Liberación L2: integración y metrología

- [ ] DS18B20 y AFEL aportan datos físicos identificables con referencia.
- [ ] Calibración de pH con patrones y validación independiente.
- [ ] Falla de sensor detectada sin dato engañoso.
- [ ] LED y web publican mismo estado del ESP32.
- [ ] SoftAP, LittleFS y HTTP funcionales sin Internet.
- [ ] Registros de integración durante ≥5 minutos.

### Liberación L3: Entrega académica

- [ ] CAD revisado y correlacionado con materiales reales.
- [ ] PPT ensayado; todos participan, tiempo y plan B según pauta aplicable.
- [ ] Informe PDF con ≤10 páginas principales, APA7, referencias y anexos A–E.
- [ ] Resultados de funcionamiento real o límites de simulación declarados.
- [ ] Enlace a código/commit válido, declaración de IA y bibliotecas.

## 6. Bitácora de incidencias técnicas ya conocidas

| Incidencia | Causa | Cambio | Evidencia deseable |
|---|---|---|---|
| Error `[::]:8000` | Dirección IPv6 no especificada mostrada por servidor | Abrir `127.0.0.1` / `localhost` y bind IPv4 | Captura antes/después |
| `Anomaly` no existe en Arduino | Macros `HIGH`, `LOW` colisionan con enum de C++ | Renombrar `ABOVE_MAX/BELOW_MIN` | Log de compilación sin errores |
| `LittleFS` no declarado | Directiva `include` sin `#` | Corregir `#include <LittleFS.h>` | Diff Git y compilación |
| `g++` no reconocido | GCC escritorio ausente del PATH | MSYS2 UCRT64 y PATH temporal | `g++ --version` |
| `.ino` vacío tras abrir en IDE | Desfase de ruta/guardado o contenido no pegado | Revisar ruta y guardar en VS Code | Sketch visible y compilación |
| Error `REAL` en servidor Python estático | `/api/readings` no existe en `http.server` | Usar `tests/mock_http_server.py` puerto 8001 | JSON accesible por HTTP |

**Nota:** una corrección que permite compilar es una iteración de software auténtica, pero no mide precisión ni robustez de sensores. Para la rúbrica de funcionamiento conviene evidenciar también iteraciones de hardware con datos cuantificados.

## 7. Reglas de mantenimiento de este registro

Tras cada reunión o experimento: registrar fecha, responsable, commit, evidencia y resultado; actualizar estado solo si hay prueba; propagar correcciones a `06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md` y al informe. No borrar riesgos resueltos; mantener historial de decisiones y resultados.
