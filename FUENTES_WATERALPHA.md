# FUENTES — WaterAlpha

## 1. Hardware actualmente seleccionado


### 1.1 Sensor de temperatura — DS18B20 waterproof

- Analog Devices — **DS18B20 Programmable Resolution 1-Wire Digital Thermometer**  
  https://www.analog.com/en/products/ds18b20.html

- DFRobot — **Waterproof DS18B20 Digital Temperature Sensor**  
  https://www.dfrobot.com/product-689.html

Implementación prevista:

- alimentación a 3,3 V;
- 1-Wire;
- pull-up aproximado de 4,7 kΩ;
- librerías OneWire + DallasTemperature.

---

### 2.2 Sensor de pH — AFEL / Kuongshun

- AFEL — **Módulo y Sonda de Detección de pH 0 a 14**  
  https://afel.cl/products/modulo-y-sonda-de-deteccion-de-ph-0-a-14-sensor-de-ph

Uso: referencia del producto local, alimentación nominal, salida analógica, conector BNC y dimensiones.

Datos declarados por la publicación consultada:

- alimentación aproximada: 5 ± 0,2 V;
- salida analógica;
- rango declarado: 0–14 pH;
- conector BNC;
- módulo aproximado: 42 × 32 × 20 mm.


## 3. Agua, calidad y límites de interpretación

### OMS

- World Health Organization — **Guidelines for drinking-water quality: fourth edition incorporating the first, second and third addenda (2026)**  
  https://www.who.int/publications/i/item/9789240121225

Uso: Corrobora la certificación de que estos paramétros no son una justificación de potabilidad y respalda nuestro marco teórico del problema.

Principio del proyecto:

> **Evaluación preliminar. No certifica potabilidad ni ausencia de patógenos.**

### EPA — referencia preliminar de pH

- United States Environmental Protection Agency — **Secondary Drinking Water Standards: Guidance for Nuisance Chemicals**  
  https://www.epa.gov/sdwa/secondary-drinking-water-standards-guidance-nuisance-chemicals

Uso: referencia preliminar del intervalo 6,5–8,5 utilizado en la máquina de estados.


### Chile

- Instituto Nacional de Normalización — **NCh409/1:2005 — Agua potable, Parte 1: Requisitos**  
  https://ecommerce.inn.cl/nch4091200545041
2: Instituto de Salud Pública de Chile. (2025). Estudio análisis de la calidad del agua cruda en servicios sanitarios rurales de la Región Metropolitana. Instituto de Salud Pública de Chile.
 https://www.ispch.cl/noticia/isp-vigila-el-agua-potable-rural-y-entrega-estudio-para-la-rm/
Uso: referencia normativa chilena.

---

## 4. Métodos y normas técnicas de medición

- ISO — **ISO 10523:2008 — Water quality — Determination of pH**  
  https://www.iso.org/standard/51994.html

- ISO — **ISO 7888:1985 — Water quality — Determination of electrical conductivity**  
  https://www.iso.org/standard/14838.html

- ISO — **ISO 7027-1:2016 — Water quality — Determination of turbidity — Part 1: Quantitative methods**  
  https://www.iso.org/standard/62801.html

- IEC — **IEC 60529 — Degrees of protection provided by enclosures (IP Code)**  
  https://webstore.iec.ch/en/publication/2452

Uso: metodología de medición, comparación con variables futuras y evitar afirmar IP67/IP68 sin ensayo.

---

## 5. Metrología y calibración

- JCGM — **JCGM 200:2012, International Vocabulary of Metrology (VIM)**  
  https://doi.org/10.59161/JCGM200-2012

- JCGM — **JCGM 100:2008, Guide to the expression of uncertainty in measurement (GUM)**  
  https://doi.org/10.59161/JCGM100-2008E

Uso: calibración, verificación, error, repetibilidad e incertidumbre.


## 6. Referencia experimental


- Giovanetti Antúnez, V. J. (2018). *Calidad fisicoquímica del agua potable en la Región Metropolitana, Chile* [Memoria de título, Universidad de Chile]. Repositorio Académico de la Universidad de Chile. https://repositorio.uchile.cl/handle/2250/151291

Uso: Permite verificar erorres y mecanismos de solucion, tanto como metodos y niveles de calibración segun un estudio teórico similar.



## 7. Software y librerías

### Arduino / ESP32

- Arduino IDE  
  https://www.arduino.cc/en/software

- Arduino Core for ESP32  
  https://docs.espressif.com/projects/arduino-esp32/en/latest/

- Instalación del core ESP32  
  https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

- Boards Manager:
  ```text
  https://espressif.github.io/arduino-esp32/package_esp32_index.json
  ```

Librerías del core utilizadas:

- `WiFi.h`
- `WebServer.h`
- `LittleFS.h`

### OneWire

- Paul Stoffregen — **OneWire**  
  https://github.com/PaulStoffregen/OneWire

Uso: comunicación 1-Wire con DS18B20.

### DallasTemperature

- Miles Burton et al. — **Arduino Temperature Control Library / DallasTemperature**  
  https://github.com/milesburton/Arduino-Temperature-Control-Library

Uso: adquisición de temperatura y gestión del DS18B20.

### LittleFS uploader

- **arduino-littlefs-upload**  
  https://github.com/earlephilhower/arduino-littlefs-upload

Uso: construcción/carga de la imagen LittleFS con la interfaz web.

---

## 8. Herramientas de desarrollo

- Visual Studio Code  
  https://code.visualstudio.com/

- GitHub  
  https://github.com/

- GitHub Desktop  
  https://desktop.github.com/

- Python 3  
  https://www.python.org/downloads/

- MSYS2  
  https://www.msys2.org/

- Autodesk Fusion 360  
  https://www.autodesk.com/products/fusion-360/

Uso: edición, control de versiones, servidor MOCK, pruebas unitarias C++ y CAD.

---

## 9. Tecnologías web utilizadas

- HTML5
- CSS3
- JavaScript vanilla
- JSON
- HTTP local



## 10. Código desarrollado/adaptado para WaterAlpha

Principales módulos:

- `WaterAlpha.ino`
- `config.h`
- `pins.h`
- `types.h`
- `controller.cpp/.h`
- `state_machine.cpp/.h`
- `mock_sensors.cpp/.h`
- `temperature_sensor.cpp/.h`
- `ph_sensor.cpp/.h`
- `ph_calibration.cpp/.h`
- `ph_pipeline.cpp/.h`
- `led_colors.cpp/.h`
- `led_rgb.cpp/.h`
- `web_server.cpp/.h`
- frontend en `web/`
- pruebas en `tests/`

Lógica propia/adaptada:

- modos MOCK/REAL;
- estados BLUE/GREEN/YELLOW/RED/MAGENTA;
- histéresis;
- pipeline de validación;
- calibración lineal de dos puntos;
- `/api/readings`;
- LED virtual/físico;
- SoftAP `WaterAlpha`.

---

## 11. Uso de inteligencia artificial

### ChatGPT — OpenAI

Aplicaciones:

- investigación preliminar;
- comparación de sensores y hardware;
- arquitectura de software;
- propuestas y revisión de código;
- debugging;
- diseño de API/JSON;
- máquina de estados e histéresis;
- testing;
- organización de fuentes;

Adaptación:

Las propuestas generadas con IA fueron revisadas y adaptadas por el equipo. La IA no se considera evidencia experimental ni sustituye datasheets, normas, pruebas físicas o documentación oficial.

El equipo debe comprender y ser capaz de explicar el código y las decisiones técnicas implementadas.


