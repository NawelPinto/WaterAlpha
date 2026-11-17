# WaterAlpha — monitoreo preliminar de pH y temperatura

**Universidad Adolfo Ibáñez · TEI201, Taller de Diseño en Ingeniería · Avance 3 (noviembre 2026)**

> **Alcance científico:** WaterAlpha NO determina potabilidad, inocuidad, microbiología ni aptitud de consumo. El prototipo supervisa exclusivamente pH y temperatura de una muestra, valida señales y expresa estados instrumentales. Verde solo significa pH dentro de la referencia experimental elegida y lecturas suficientemente estables/validadas, no agua segura.

## 1. Problema, usuarios y objetivo

El usuario objetivo es una persona senderista que necesita información instrumental preliminar acerca de una muestra de agua, más allá de la apariencia visual. Esta etapa Alpha implementa dos parámetros: **pH** (adimensional) y **temperatura** (°C). Conductividad (µS/cm) y turbidez (NTU) son futuras ampliaciones, no implementadas en este prototipo. WaterAlpha informa y supervisa; **no trata, purifica ni controla el agua**.

**Integrantes del equipo:** [Completar nombres y roles reales antes de entregar].

## 2. Arquitectura de funcionamiento

```text
Muestra de agua
  ├─ Electrodo de pH + acondicionador AFEL (~5 V, modelo eléctrico por verificar)
  │       └─ adaptación de señal verificada → ADC1 del ESP32 (GPIO por confirmar)
  └─ DS18B20 impermeable → 1-Wire (GPIO por confirmar, pull-up 4,7 kΩ)
           ↓
   Placa UAI con Ai-Thinker ESP-32S (alimentación USB)
           ↓
   Validación de señales, calibración pH, estabilidad temporal
           ↓
   Máquina de estados con referencia preliminar pH 6,5–8,5
           ├─ Tres LED discretos rojo / verde / amarillo
           └─ HTTP GET /api/readings → Wi-Fi SoftAP → dashboard sin Internet
                                             └─ script Python en PC → CSV / JSONL
```

Se utilizan cinco **estados lógicos**: `BLUE` (inicio), `GREEN` (pH en referencia y lectura estable), `YELLOW` (precaución/inestabilidad), `RED` (pH fuera de referencia), `MAGENTA` (error/no verificado). Como el hardware físico propuesto tiene **tres LED independientes**, la traducción es: verde fijo; amarillo fijo; rojo fijo; amarillo parpadeo lento (inicio); rojo parpadeo rápido (error). Estos parpadeos deben observarse en placa antes de declararse probados.

**Temperatura:** se muestra y se comprueba su plausibilidad y estabilidad en ventana; no se asigna arbitrariamente un «rango potable» de temperatura. El firmware propuesto usa como *hipótesis de ensayo* una amplitud máxima de 0,5 °C en cinco lecturas válidas. Debe verificarse experimentalmente.

## 3. Hardware y requisitos eléctricos

| Componente | Uso | Confirmación pendiente |
|---|---|---|
| Placa UAI, Ai-Thinker ESP-32S | MCU, Wi-Fi SoftAP, ADC1, GPIO | Esquema/pinout reales; Flash, alimentación y pines libres |
| DS18B20 encapsulado | Temperatura (°C), bus 1-Wire | Polaridad por ficha, GPIO y comparación de referencia |
| Módulo AFEL + electrodo | pH a través de salida analógica | Identificar módulo, alimentación, rango AO, circuito de adaptación y calibración |
| Resistencias 4,7 kΩ + resistencias divisor | Pull-up DS18B20 + adaptación analógica | Medir valores reales, confirmar kΩ/Ω y dimensionar |
| Tres LED separados rojo, verde y amarillo | Indicación física de estado | Cada LED exige resistencia propia, GPIO disponible, polaridad y corriente |
| Dos protoboards y Dupont | Montaje de pruebas | Revisar conectividad real y cantidad suficiente de puentes |
| Carcasa impresa + cámara de muestra | Soporte y separación húmedo/seco | Modelo CAD real, sellado, agujeros y montaje; no afirmar grado IP |

**Nunca** conectar la salida AO del módulo pH sin comprobar tensión y polaridad; la entrada del ESP32 opera a niveles de 3,3 V, no 5 V. Un divisor solo sirve si el nivel es positivo y adecuado para el ADC; puede afectar la carga, ruido y precisión. Para 3 LED discretos debe existir una resistencia limitadora **en serie con cada LED**. No conectar directamente los LED a los GPIO.

**Estados de aceptación:** revisar `docs/PLAN_DE_TRABAJO_Y_RIESGOS.md` y `hardware/conexiones_preliminares.md`. El archivo `pins.h` de esta rama vuelve a valores `-1` y `PINOUT_CONFIRMED=false` como **bloqueo preventivo**; debe completarse únicamente con la placa real verificada.

## 4. Cómo ver el dashboard en el computador sin sensores

Requisitos: navegador web moderno y Python 3. Desde la raíz del repositorio:

```powershell
cd web
python -m http.server 8000 --bind 127.0.0.1
```

Abrir `http://127.0.0.1:8000/`. `web/config.js` utiliza `API_MODE:"MOCK"` y muestra controles de simulación. **Es una demostración sintética del sitio, no una medición real.** Si no abre el enlace, usar expresamente la dirección `127.0.0.1`.

Para probar el servidor HTTP sintético del repositorio, ejecutar `python tests/mock_http_server.py` y seleccionar `API_MODE:"REAL"` en una copia del sitio; el JSON recibido seguirá señalando `mode:"MOCK"` si la fuente es simulada.

## 5. Cómo conectar la web al ESP32

**Previo obligatorio:** aprobar alimentación, modelo AFEL, GPIO, resistencias e inspección del circuito; disponer de las librerías Arduino OneWire y DallasTemperature y el core ESP32 de Espressif. Abrir `firmware/WaterAlpha/WaterAlpha.ino` en Arduino IDE y compilar. Primero se recomienda `MOCK_MODE=true`, `USE_REAL_PH=false`, `USE_REAL_TEMPERATURE=false` y `USE_REAL_LED=false` en `config.h`.

1. Desde la raíz ejecutar `python scripts/sync_web.py` para copiar la web a `firmware/WaterAlpha/data/`. En `web/` conservará MOCK; en `data/` cambiará a **REAL**.
2. Seleccionar esquema de particiones compatible con la capacidad Flash **comprobada** en la placa UAI.
3. **Compilar y subir firmware**, y de forma separada **construir y subir la imagen LittleFS** mediante el complemento compatible con Arduino IDE.
4. Abrir monitor serial a `115200`; confirmar `[WIFI]`, dirección IP anunciada y `[FS] LittleFS montado`.
5. Conectar PC/teléfono al SSID `WaterAlpha` (contraseña de desarrollo en `config.h`; cambiar antes de uso externo). Abrir `http://<IP_del_serial>/`, a menudo `192.168.4.1` si así lo anuncia el dispositivo.
6. Revisar `http://<IP_del_serial>/api/readings`, `mode`, `reading_valid`, `calibrated`, `ph_sensor_ok` y `temperature_sensor_ok`. **`mode:REAL` no implica que el pH sea válido.**
7. Tras verificar cada sensor y obtener los patrones de calibración, activar banderas en forma escalonada y registrar las evidencias. Una instalación sin calibración completa debe mantener `MAGENTA` o diagnóstico inválido.

La placa puede servir la web por SoftAP sin conexión a Internet. El cable USB entrega alimentación desde el PC/fuente autorizada, no una red de datos de Internet. Revisar compatibilidad de cada puerto y consumo bajo Wi-Fi.

## 6. Registro persistente de datos, fecha e iteración

Desde el PC conectado al SoftAP y en la raíz del repo, ejecutar:

```powershell
python scripts/record_readings.py --url http://192.168.4.1/api/readings --interval 2
```

El programa crea **tres archivos por sesión** en `data/registros/AAAA-MM-DD/`: `.csv` (tabla), `.jsonl` (respuesta cruda y metadata) y `.meta.json` (contexto de sesión). Cada registro contiene `iteration`, fecha/hora local y UTC del PC, pH, °C, fuente, diagnóstico, flags y antigüedad. Para terminar, `Ctrl+C`.

**Prueba deliberada con simulación:** agregar `--allow-mock` y usar un servidor API simulado; las filas se etiquetan **SIMULATED**. Por defecto, el script **no graba muestras MOCK**. Los fallos HTTP se guardan como incidencias; no inventan valores de sensores. Ajustar la fecha/hora de Windows antes de la sesión; el ESP32 sin RTC no garantiza fecha civil.

El registro es persistente **en el disco del computador**; no se sube automáticamente al repositorio remoto. Para respaldarlo en GitHub, después de recuperar Internet:

```powershell
git status
git add data/registros/
git commit -m "Registra sesiones de mediciones WaterAlpha"
git push
```

Comprobar que `.gitignore` no excluya `data/registros/*.csv` o `*.jsonl` si se desean guardar en Git. El evaluador puede revisar los datos aunque no haya Internet durante la toma, siempre que los archivos locales estén accesibles.

## 7. Validación y criterios del Avance 3

El documento obligatorio es `testing/protocolo_avance3.md`. Contiene variables medidas, controladas y derivadas, método paso a paso, condiciones, cantidades por ensayo, instrumentos, indicadores, criterios previos y formato de evidencia. Los resultados que aún no existan se dejarán como **PENDIENTE**, sin inventar tablas experimentales. Consultar `docs/VARIABLES_Y_ESTADOS.md` para interpretar umbrales e indicadores.

## 8. Estructura del repositorio

```text
README.md                  # Este documento
FUENTES.md                 # Declaración IA/librerías del equipo; agregar archivo existente
firmware/WaterAlpha/       # Sketch Arduino y módulos C++
firmware/WaterAlpha/data/  # Web en LittleFS (API_MODE REAL)
web/                      # Web de desarrollo (API_MODE MOCK)
scripts/                  # sync_web.py y record_readings.py
hardware/                 # conexiones preliminares + BOM; actualizar a esquemático real
diseño-3d/                 # Archivos .f3d/.f3z y planos/renders reales (agregar)
testing/                   # protocolo experimental
docs/                     # variables, riesgos, guion y auditoría
data/registros/           # datos recolectados y etiquetados por sesión
tests/                    # pruebas unitarias y pruebas existentes
```

**Nota de integridad académica:** `FUENTES.md` debe contener librerías y uso de IA; agregar el archivo del equipo **antes de subir**. Este paquete no sustituye los archivos originales de Fusion 360 ni un esquemático definitivo, que requieren mediciones y verificación física. Los tests unitarios de C++ y Python no equivalen a validación del hardware.

## 9. Estado del desarrollo declarado al 16 de noviembre de 2026

- **Disponible en los archivos recibidos:** arquitectura Arduino, FSM de cinco estados, pipeline de pH, sensor DS18B20, lectura ADC, servidor HTTP/SoftAP, prototipo de carcasa impreso, estructura de carpetas y pruebas reportadas del software.
- **Implementado en este paquete de propuesta:** panel web centrado en pH/°C, modo de adquisición explícito, sincronizador LittleFS, logging persistente CSV/JSONL con fecha e iteración, propuesta para tres LED separados y ventana térmica de estabilidad.
- **No validado por el paquete:** compilación en el hardware UAI, seguridad del circuito, pH calibrado en campo, continuidad del Wi-Fi real, exactitud, secado/sellado, autonomía, LED físico o desempeño durante toda la demostración.

## 10. Referencias y fuentes

Véase **`FUENTES.md` del equipo**; no copiar especificaciones no verificadas de modelos diferentes. Como fuentes técnicas primarias para revisar el diseño están el fabricante Analog Devices (DS18B20), Espressif (ESP32/ADC y GPIO) y la publicación del producto AFEL/Kuongshun. Toda afirmación de exactitud deberá corresponder al módulo exacto y a su prueba experimental.
**Nota de actualización GitHub (16/11/2026):** En esta rama se revisó el código web original de `main` y se aplicaron modificaciones incrementales al dashboard. Las comprobaciones de integración física, calibración pH y CAD real continúan pendientes; no se acreditan con archivos Markdown.
