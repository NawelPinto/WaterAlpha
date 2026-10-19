# Matriz de trazabilidad — Requisitos, evidencias y cuatro entregables


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

## 7. Revisión de coherencia entre entregables


