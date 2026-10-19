# Registro de riesgos, pendientes y plan de liberación — WaterAlpha

## 1. Dependencias que bloquean las pruebas físicas

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


## 2. Criterios de liberación por hito

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
