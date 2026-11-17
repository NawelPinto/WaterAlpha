# Guion breve de exposición WaterAlpha — 8 a 10 minutos

**Duración oficial de la pauta:** 8–10 min + 3 min de preguntas. **Presentación voluntaria del 17/11/2026:** el docente flexibiliza pruebas definitivas; mantiene exigencia de funcionamiento y protocolo. No sustituir demostración real por video ni MOCK oculto.

| Bloque | Duración orientativa | Qué mostrar / qué explicar |
|---|---:|---|
| 1. Problema y cambio de alcance | 0:50 | Senderistas no pueden inferir composición del agua por apariencia; WaterAlpha ahora mide solo pH y temperatura. No dice potable/no potable |
| 2. Arquitectura sensor→decisión→respuesta | 1:20 | Diagrama con AFEL→ADC1, DS18B20→1-Wire, placa UAI, máquina de estados, LED y SoftAP/web/logger; rotular MOCK/REAL |
| 3. Hardware y conexiones | 1:20 | Montaje seco/seguro, sensores, divisor condicionado por AO, pull-up 4,7 kΩ y resistencias individuales para cada LED; especificar GPIO confirmados o pendientes |
| 4. CAD físico | 1:20 | Carcasa morada + modelo Fusion real, cotas, interior/cortes, separación húmedo-seco, accesos, mantenimiento y limitaciones del prototipo abierto |
| 5. Demo operativa | 2:00 | Enlace web, API, datos con unidades y etiqueta real/mock; mostrar LED físico y coincidencia, registro CSV por iteración; si parte no funciona, declarar estado real |
| 6. Testing/validación | 1:40 | Mapa de ensayos T01–T08, buffers pH 2+1, termómetro referencia, tolerancias, integración 300 s, muestras y análisis MAE |
| 7. Repositorio y cierre | 0:50 | README, FUENTES, hardware, diseño-3d, testing, docs, firmware, script de logging, próximos riesgos críticos |
| **Total** | **9:20** | Ensayar transiciones y repartir roles reales entre integrantes |

## Diálogo sugerido para el cambio de alcance

> «Inicialmente propusimos evaluar cuatro parámetros. Por restricciones presupuestarias y de disponibilidad, concentramos el Alpha en pH y temperatura. Esto aumenta la posibilidad de validar la cadena completa, pero limita deliberadamente la interpretación: no identificamos patógenos ni certificamos potabilidad. El verde expresa únicamente que nuestras lecturas cumplen los requisitos instrumentales y el pH cae en la referencia preliminar elegida».

## Demostración A/B honesta

- **Plan A (integración física):** al comenzar mostrar foto/placa con cableado verificado, conectar SoftAP, abrir la API con `mode:REAL`, mostrar temperatura de DS18B20; si pH validado con buffers, mostrar pH y estados físicos. Ejecutar logger Python y abrir CSV real. Si no hay calibración, pH quedará nulo/MAGENTA: explicarlo sin encubrirlo.
- **Plan B (solo simulación):** sin alimentación insegura, iniciar servidor local web `python -m http.server 8000 --bind 127.0.0.1`, mostrar la UI con `SIMULACIÓN` visible, mover sliders y comprobar la FSM. Mostrar pruebas C++/Python y protocolo. **No se presenta como reemplazo suficiente de la demostración real exigida; es una evidencia parcial.**

## Preguntas probables

**¿El agua con verde se puede beber?** No. Verde significa que el pH está dentro de una referencia instrumental y que ambas señales cumplen validación/estabilidad; no medimos microorganismos ni contaminantes específicos.

**¿Por qué no usan cuatro sensores?** Costos, disponibilidad y necesidad de concentrar recursos en un Alpha reproducible y calibrable; CE y NTU pasan a alcance futuro, documentados sin falsear que hoy se miden.

**¿Qué mide realmente la temperatura?** Grados Celsius de la muestra con DS18B20 digital. Ayuda a caracterizarla y observar estabilidad; no se usa un supuesto límite sanitario universal de temperatura.

**¿Cómo se calibra pH?** Se miden milivoltios ADC tras adaptar la señal AFEL, dos buffers para ajustar una recta y un tercero independiente para error residual. Sin soluciones patrón no declaramos lectura válida.

**¿Por qué ADC1?** En el ESP32 clásico, ADC2 entra en conflicto con Wi-Fi; ADC1 es preferible, siempre que el GPIO esté accesible en la placa UAI.

**¿Tres LED y cinco estados?** Verde/amarillo/rojo fijos y parpadeos de amarillo para iniciar, rojo para error. Distintos del viejo LED RGB de cuatro terminales; se necesita verificar resistencias y cables.

**¿Cómo guardan datos sin Internet?** El ESP32 crea su propia red local y sirve la API; Python en un PC conectado escribe CSV/JSONL con fecha/iteración en el disco, y luego podemos subir esos archivos a GitHub cuando volvemos a Internet.

**¿La carcasa morada es impermeable?** No se ha validado estanqueidad. Es una iteración física del soporte; falta verificar unión al CAD real, pasamuros, separación húmedo/seco y mantenimiento.

**¿Cuál es el principal riesgo?** Obtener pH válido depende del modelo AFEL, el rango de voltaje AO, la adaptación al ADC y patrones de calibración; se han establecido pruebas previas para no dañar placa ni reportar lecturas falsas.