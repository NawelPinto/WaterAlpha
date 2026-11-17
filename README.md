# WaterAlpha — monitoreo preliminar de pH y temperatura

**Universidad Adolfo Ibáñez · TEI201, Taller de Diseño en Ingeniería · Avance 3 (noviembre 2026)**

> **Alcance científico:** WaterAlpha NO determina potabilidad, ni aptitud de consumo. El prototipo supervisa exclusivamente pH y temperatura de una muestra, valida señales y expresa estados instrumentales. Verde solo significa pH dentro de la referencia experimental elegida y lecturas suficientemente estables/validadas, no agua segura.

## 1. Problema, usuarios y objetivo

El usuario objetivo es una persona senderista que necesita información instrumental preliminar acerca de una muestra de agua, más allá de la apariencia visual. Esta etapa Alpha implementa dos parámetros: **pH** (adimensional) y **temperatura** (°C). Conductividad (µS/cm) y turbidez (NTU) son futuras ampliaciones, no implementadas en este prototipo. WaterAlpha informa y supervisa; **no trata, purifica ni controla el agua**.

**Integrantes del equipo:** [Sofía Kunihiro, Florencia Venegas, Simón, Aníbal Pinto].

## 2. Arquitectura de funcionamiento

```text
Muestra de agua
  ├─ Electrodo de pH + acondicionador AFEL (~5 V, modelo eléctrico por verificar)
  │       └─ adaptación de señal verificada → ADC1 del ESP32 (GPIO 34)
  └─ DS18B20 impermeable → 1-Wire (GPIO 25, pull-up 4,7 kΩ)
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

## 4. Cómo ver el dashboard en el computador sin sensores

Requisitos: navegador web moderno y Python 3. Desde la raíz del repositorio:

```powershell
cd web
python -m http.server 8000 --bind 127.0.0.1
```

Abrir `http://127.0.0.1:8000/`. `web/config.js` utiliza `API_MODE:"MOCK"` y muestra controles de simulación. **Es una demostración sintética del sitio, no una medición real.** Si no abre el enlace, usar expresamente la dirección `127.0.0.1`.

Para probar HTTP del repositorio preexistente, ejecutar `python tests/mock_http_server.py` si ese archivo está disponible y seleccionar `API_MODE:"REAL"` en una copia del sitio; el JSON recibido seguirá señalando `mode:"MOCK"` si la fuente es simulada.

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



Comprobar que `.gitignore` no excluya `data/registros/*.csv` o `*.jsonl` si se desean guardar en Git. El evaluador puede revisar los datos aunque no haya Internet durante la toma, siempre que los archivos locales estén accesibles.

## 6. Validación y criterios del Avance 3

El documento obligatorio es `testing/protocolo_avance3.md`. Contiene variables medidas, controladas y derivadas, método paso a paso, condiciones, cantidades por ensayo, instrumentos, indicadores, criterios previos y formato de evidencia. Los resultados que aún no existan se dejarán como **PENDIENTE**, sin inventar tablas experimentales. Consultar `docs/VARIABLES_Y_ESTADOS.md` para interpretar umbrales e indicadores.

## 7. Estructura del repositorio

```text
README.md                  # Este documento
FUENTES.md                 # Declaración IA/librerías del equipo; agregar archivo existente
firmware/WaterAlpha/       # Sketch Arduino y módulos C++
firmware/WaterAlpha/data/  # Web en LittleFS (API_MODE REAL)
web/                      # Web de desarrollo (API_MODE MOCK)
scripts/                  # sync_web.py y record_readings.py
hardware/                 # conexiones preliminares + BOM; actualizar a esquemático real
diseño-3d/                 # Archivos .f3d/.f3z y planos/renders reales 
testing/                   # protocolo experimental
docs/                     # variables, riesgos, guion y auditoría
data/registros/           # datos recolectados y etiquetados por sesión
tests/                    # pruebas unitarias y pruebas existentes
```

**Nota de integridad académica:** `FUENTES.md` debe contener librerías y uso de IA; agregar el archivo del equipo **antes de subir**. Este paquete no sustituye los archivos originales de Fusion 360 ni un esquemático definitivo, que requieren mediciones y verificación física. Los tests unitarios de C++ y Python no equivalen a validación del hardware.

## 8. Estado del desarrollo declarado al 16 de noviembre de 2026

### Estado de desarrollo — Avance 3

WaterAlpha dispone de una arquitectura basada en un microcontrolador ESP32, un sensor digital de temperatura DS18B20 y un módulo analógico de pH AFEL. El equipo ha desarrollado el encapsulado CAD y realizado ensayos experimentales con los sensores físicos.

El sensor de pH fue calibrado mediante un modelo lineal de dos puntos, utilizando referencias de pH 4,19 y 9,38, asociadas a lecturas ADC de 2194,00 y 2031,29 mV, respectivamente.

La comprobación con un tercer patrón de pH 7,09 y una lectura de 2113,00 mV produjo una estimación de pH 6,774, equivalente a un error absoluto observado de aproximadamente 0,316 unidades de pH.

El firmware incorpora adquisición de datos, procesamiento, diagnóstico de estados y un servidor HTTP local. La interfaz web permite visualizar pH, temperatura y resultados de una evaluación experimental que no determina potabilidad.

El sistema contempla la incorporación de tres indicadores luminosos independientes (verde, amarillo y rojo), cuya implementación física requiere confirmar GPIO disponibles y disponer de resistencias limitadoras adecuadas.

El proyecto se encuentra en fase de integración y preparación de la validación experimental definitiva. La evidencia de los ensayos físicos, los archivos originales CAD y los resultados de pruebas deben mantenerse vinculados a la documentación y al repositorio.

## 9. Referencias y fuentes

Véase **`FUENTES.md` del equipo**; no copiar especificaciones no verificadas de modelos diferentes. Como fuentes técnicas primarias para revisar el diseño están el fabricante Analog Devices (DS18B20), Espressif (ESP32/ADC y GPIO) y la publicación del producto AFEL/Kuongshun. Toda afirmación de exactitud deberá corresponder al módulo exacto y a su prueba experimental.
