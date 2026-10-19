# Auditoría y guion de videotutorial — Replicar WaterAlpha desde GitHub

**Audiencia:** integrante nuevo o persona que trabaja desde otro computador Windows; sección adicional para cargarlo luego en **otro ESP32 físico**.  
**Estado del proyecto:** desarrollo sin hardware concluido en buena parte; el despliegue en ESP32 real y la calibración física **no están probados**.  
**Versión del tutorial:** 1.0, 18-10-2026.  
**Objetivo:** reproducir el entorno, clonar el mismo commit, instalar dependencias, probar frontend, C++, API HTTP, firmware Arduino y LittleFS; no afirmar ejecución física hasta realizarla.

## 0. Antes de grabar: preparar lo que se mostrará

- [ ] Obtener URL del repositorio **real** `WaterAlpha` en GitHub y confirmar permisos de acceso. No se dispone de esa URL en esta auditoría; usar `[URL_REPO]` como marcador.
- [ ] Confirmar el `commit` que se desea replicar (`git log -1 --oneline` o History de GitHub Desktop) y anotar **hash**. Grabar el mismo commit de principio a fin; no editar el código sin crear nuevo commit.
- [ ] Comprobar árbol real y nombres idénticos a lo documentado; `docs/` puede contener archivos adicionales.
- [ ] Crear carpeta `evidencias/tutorial/` o ubicación equivalente (si se incorpora al repo, revisar tamaño y privacidad). No subir ejecutables generados ni claves reales.
- [ ] Disponer de conexión a Internet para **instalación inicial** de programas, Arduino core y librerías; durante ejecución final el SoftAP no necesita Internet.
- [ ] Mostrar rutas de ejemplo en Windows; `C:\Dev\WaterAlpha` es la ruta de la máquina original, **no requisito**.

## 1. Instalaciones: cuadro completo

| Componente | Necesario para | Instalación/verificación | Observación |
|---|---|---|---|
| **GitHub Desktop** | Clonar, commits y push | Sitio oficial `https://desktop.github.com/`; iniciar sesión y clonar | Usar repo privado si corresponde; **Fetch** no equivale a **Push** |
| **Visual Studio Code** | Edición de HTML/CSS/JS/C++/Python/Markdown | `https://code.visualstudio.com/`; abrir carpeta raíz | No es compilador Arduino; extensiones opcionales |
| **Python 3** | Servidor local y API de prueba | `https://www.python.org/downloads/`; `python --version` | Comprobar PATH/alias de Windows |
| **Arduino IDE 2** | Compilar/subir sketch ESP32 | `https://www.arduino.cc/en/software`; abrir `.ino` | Seleccionar target provisional `ESP32 Dev Module` solo mientras se verifica placa real |
| **Paquete ESP32 by Espressif Systems** | Core Arduino del ESP32 | URL del Gestor de tarjetas + buscar `esp32` | En log anterior figuraba `esp32 3.3.12`, pero **verificar** versión real de la nueva PC |
| **OneWire** — Paul Stoffregen | DS18B20, bus digital | Gestor de bibliotecas Arduino | Instalar librería de autor correcto |
| **DallasTemperature** — Miles Burton y colaboradores | Adquisición °C desde DS18B20 | Gestor de bibliotecas Arduino | Depende de OneWire |
| **MSYS2 UCRT64 + GCC** | Tests unitarios `.exe` de Windows | `https://www.msys2.org/` y `pacman` | GCC de escritorio distinto del toolchain ESP32 |
| **arduino-littlefs-upload** (`.vsix`) | Crear/cargar imagen LittleFS | `https://github.com/earlephilhower/arduino-littlefs-upload` | Guardar VSIX en `%USERPROFILE%\.arduinoIDE\plugins\` y reiniciar Arduino IDE |
| **Navegador Chromium/Firefox** | Visualización de interfaz web | Navegador compatible con JS moderno | Evitar abrir HTML con `file://` para pruebas de API |
| **Autodesk Fusion 360** (opcional para software; importante para CAD) | Abrir y revisar modelo 3D | Instalación bajo licencia académica correspondiente | Requiere enlace al CAD nativo del equipo; no se encontró archivo CAD adjunto |
| **Multímetro + drivers USB-UART** (solo al recibir hardware) | Reconocer puerto COM y validar tensiones | Dependiente del chip USB/placa UAI | No instalar driver al azar; identificar puente USB-UART real |

**ESP32 Boards Manager URL oficial utilizada en el desarrollo:**

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

En Arduino IDE 2: **Archivo → Preferencias → URLs adicionales del gestor de tarjetas** (añadir sin borrar otras URLs), luego **Gestor de tarjetas → esp32 by Espressif Systems → instalar**.

## 2. Escena 1 — Clonar y reconocer carpetas

**Capturar:** GitHub Desktop → File → Clone repository → elegir WaterAlpha → `Local path` (p. ej. `C:\Dev\WaterAlpha`), esperar → Repository → Open in Visual Studio Code.

**Revisar (rutas relativas a la raíz):**

```text
WaterAlpha/
├── .gitignore
├── web/
│   ├── index.html
│   ├── style.css
│   ├── config.js
│   ├── mock.js
│   ├── state_machine.js
│   ├── api.js
│   └── app.js
├── firmware/
│   └── WaterAlpha/
│       ├── WaterAlpha.ino
│       ├── config.h
│       ├── pins.h
│       ├── types.h
│       ├── mock_sensors.{h,cpp}
│       ├── controller.{h,cpp}
│       ├── state_machine.{h,cpp}
│       ├── temperature_sensor.{h,cpp}
│       ├── ph_sensor.{h,cpp}
│       ├── ph_calibration.{h,cpp}
│       ├── ph_pipeline.{h,cpp}
│       ├── led_colors.{h,cpp}
│       ├── led_rgb.{h,cpp}
│       ├── web_server.{h,cpp}
│       └── data/ [copia web para LittleFS]
├── tests/
│   ├── test_state_machine.cpp
│   ├── test_ph_calibration.cpp
│   ├── test_ph_pipeline.cpp
│   ├── test_system_flow.cpp
│   ├── test_led_colors.cpp
│   └── mock_http_server.py
└── docs/
```

**Aclaración de auditoría:** esta estructura refleja el código descrito durante el desarrollo y debe **compararse con la clonación real** antes de grabarla como verdadera. No incluir archivos `.exe` y `.bin` de build salvo que el equipo decida archivarlos como releases verificadas; actualmente `.gitignore` excluye los generados.

## 3. Escena 2 — Probar la web local en MOCK (sin Arduino ni placa)

Dentro de `web/config.js` verificar:

```javascript
API_MODE: "MOCK",
API_REFRESH_MS: 1500,
API_TIMEOUT_MS: 3000,
```

En VS Code → Terminal → Nueva terminal (PowerShell):

```powershell
cd C:\Dev\WaterAlpha\web
python -m http.server 8000 --bind 127.0.0.1
```

Abrir `http://127.0.0.1:8000/`, mover sliders pH y temperatura, casillas sensor/estabilidad/calibración, comprobar cambios de LED, panel JSON y texto de advertencia. Para cerrar: `Ctrl+C` en terminal.

**Problema ya ocurrido en desarrollo:** Windows/VS Code abrió el vínculo `http://[::]:8000/`, generando `ERR_ADDRESS_INVALID`. **Usar `http://localhost:8000/` o `http://127.0.0.1:8000/`**, no la dirección no especificada IPv6 `::`.

**Captura:** pH 7,2 GREEN; >8,5 RED; sensor falso MAGENTA, etiquetado «simulado».

## 4. Escena 3 — HTTP con `API_MODE:"REAL"` pero sensores MOCK

**Explicar claramente los dos significados de REAL:**

- `web/config.js` en MOCK = la web obtiene JSON de `window.waterAlphaMock`.
- `firmware/WaterAlpha/data/config.js` en REAL = la web realiza `fetch('/api/readings')` vía HTTP.
- El contenido de `mode:"MOCK"` en respuesta JSON = las mediciones proceden de una simulación, aunque HTTP sea una conexión real.
- Los booleanos `USE_REAL_PH`, `USE_REAL_TEMPERATURE`, `USE_REAL_LED` en `firmware/WaterAlpha/config.h` controlan el **hardware físico** de cada subsistema (mantener falsos sin placa).

Ejecutar desde **raíz** del repo:

```powershell
cd C:\Dev\WaterAlpha
python tests/mock_http_server.py
```

Abrir `http://127.0.0.1:8001/`; observar secuencia de colores en intervalos de 5 s, panel JSON y ausencia de controles del simulador. Comprobar en otra pestaña `http://127.0.0.1:8001/api/readings`.

**Aclaración de evidencia:** el servidor Python simula las respuestas según escenarios; no ejecuta `WAController`, el `WebServer` del ESP32 ni LittleFS física. Demuestra interfaz/API de prueba, no prototipo IoT operando.

## 5. Escena 4 — Instalar MSYS2 y compilar los cinco tests

1. Descargar/instalar MSYS2, abrir **MSYS2 UCRT64**, ejecutar:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
```

2. En PowerShell de VS Code (ajustar ruta de instalación si diferente):

```powershell
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"
g++ --version
```

3. Desde la **raíz del repo**, compilar y ejecutar en este orden:

**A. Máquina de estados (11 casos):**

```powershell
g++ -std=c++17 -Wall -Wextra -I firmware/WaterAlpha tests/test_state_machine.cpp firmware/WaterAlpha/state_machine.cpp -o tests/test_state_machine.exe
.\tests\test_state_machine.exe
```

**B. Calibración matemática (12 casos sintéticos):**

```powershell
g++ -std=c++17 -Wall -Wextra -I firmware/WaterAlpha tests/test_ph_calibration.cpp firmware/WaterAlpha/ph_calibration.cpp -o tests/test_ph_calibration.exe
.\tests\test_ph_calibration.exe
```

**C. Pipeline pH (5 casos):**

```powershell
g++ -std=c++17 -Wall -Wextra -I firmware/WaterAlpha tests/test_ph_pipeline.cpp firmware/WaterAlpha/ph_pipeline.cpp firmware/WaterAlpha/ph_calibration.cpp -o tests/test_ph_pipeline.exe
.\tests\test_ph_pipeline.exe
```

**D. Flujo integrado portable (17 comprobaciones):**

```powershell
g++ -std=c++17 -Wall -Wextra -I firmware/WaterAlpha tests/test_system_flow.cpp firmware/WaterAlpha/mock_sensors.cpp firmware/WaterAlpha/state_machine.cpp firmware/WaterAlpha/ph_calibration.cpp firmware/WaterAlpha/ph_pipeline.cpp -o tests/test_system_flow.exe
.\tests\test_system_flow.exe
```

**E. Mapeo RGB (5 casos):**

```powershell
g++ -std=c++17 -Wall -Wextra -I firmware/WaterAlpha tests/test_led_colors.cpp firmware/WaterAlpha/led_colors.cpp -o tests/test_led_colors.exe
.\tests\test_led_colors.exe
```

**Consolas esperadas:** `WaterAlpha: pruebas OK`; `WaterAlpha: 12 pruebas de calibracion OK`; `WaterAlpha: 5 pruebas de pipeline OK`; `WaterAlpha: 17 comprobaciones integradas OK`; `WaterAlpha: 5 pruebas LED OK`. **Registrar salidas reales**; no inventarlas por coincidir con ejemplos. Si el código no coincide con el repo actual, depurar antes de grabar.

**Importante:** estos tests compilan con GCC de escritorio porque los módulos de lógica no dependen de `Arduino.h`. Los módulos `controller.cpp`, sensores/LED físicos y `WebServer` no se ejecutan aquí.

## 6. Escena 5 — Compilar firmware Arduino/C++ ESP32

1. Arduino IDE 2 instalado y paquete `esp32 by Espressif Systems` habilitado; registrar versión visible (el log de desarrollo mostró core **3.3.12**, no asumir actualización automática compatible).
2. Gestor de bibliotecas: instalar **OneWire**, **DallasTemperature**. `WiFi.h`, `WebServer.h`, `LittleFS.h` vienen con el core ESP32 seleccionado; no confundir con librerías de placas distintas.
3. **Archivo → Abrir** `C:\Dev\WaterAlpha\firmware\WaterAlpha\WaterAlpha.ino`. La carpeta del sketch debe llamarse **WaterAlpha** igual que `.ino`. Arduino IDE compila los `.cpp` de esa carpeta.
4. **Herramientas → Placa → ESP32 Dev Module** provisional. Sin hardware no seleccionar COM ni pulsar «Subir».
5. Confirmar antes de compilar:

```cpp
// firmware/WaterAlpha/config.h
constexpr bool USE_REAL_TEMPERATURE = false;
constexpr bool USE_REAL_PH = false;
constexpr bool USE_REAL_LED = false;

// firmware/WaterAlpha/pins.h
constexpr bool PINOUT_CONFIRMED = false;
constexpr int PIN_TEMP = -1;
constexpr int PIN_PH_ADC = -1;
constexpr int PIN_LED_R = -1;
constexpr int PIN_LED_G = -1;
constexpr int PIN_LED_B = -1;
```

6. Pulsar **✓ Verificar**. Guardar screenshot de resultado (flash/RAM, board/core). Compilar no verifica físicamente Wi-Fi ni sensores.

**Dos errores de desarrollo que conviene mostrar en video como debugging real:**

- `state_machine.h`: `enum class Anomaly { HIGH, LOW }` colisionaba con macros `HIGH/LOW` del core Arduino. Solución: `ABOVE_MAX`, `BELOW_MIN` en `.h` y `.cpp`.
- `web_server.cpp`: estaba escrito `include <LittleFS.h>` sin `#`, generando cascada de errores C++; solución `#include <LittleFS.h>`.

**También ocurrido:** `WaterAlpha.ino` se abrió inicialmente vacío en Arduino; verificar que el archivo y su copia en VS Code sean el mismo, guardado con `Ctrl+S`, cerrar/reabrir sketch si hay desfase.

## 7. Escena 6 — Preparar archivos LittleFS y generar imagen

1. En carpeta del sketch crear/verificar `data/` con `index.html`, `style.css`, `config.js`, `mock.js`, `state_machine.js`, `api.js`, `app.js`.
2. **Copiar**, no mover, desde `web/`. En `firmware/WaterAlpha/data/config.js` conservar `API_MODE:"REAL"`.
3. Complemento **arduino-littlefs-upload**: descargar `.vsix` desde el repositorio oficial, colocar en `%USERPROFILE%\.arduinoIDE\plugins\`, reiniciar IDE. Si no aparece, comprobar versión IDE y ruta de usuario/instalación.
4. En Arduino IDE seleccionar **Partition Scheme** compatible con capacidad física verificada; sin placa usar esquema provisional de ESP32 y **no intentar upload**.
5. `Ctrl+Shift+P` → **Build LittleFS image in sketch directory**. Esperar el archivo `firmware/WaterAlpha/mklittlefs.bin`. Comprobarlo:

```powershell
Get-Item .\firmware\WaterAlpha\mklittlefs.bin
```

6. Recordar que **Build** crea el archivo; **Upload LittleFS** lo transfiere a la memoria de una placa y depende del puerto, versión y esquema. `mklittlefs.bin` puede ser ignorado por Git (`.gitignore`) ya que es artefacto reconstruible. Rehacerlo si cambia `data/`.

## 8. Escena 7 — Explicar la lógica y los conceptos sin leer todo el código

**Términos clave que el video debe enseñar:**

- `setup()` Arduino: arranque único; `loop()`: ciclo continuo, mantiene `WebServer::handleClient()`.
- SoftAP: ESP32 crea red local propia, independiente de router, Internet o nube.
- LittleFS: filesystem en partición Flash; su upload no es el mismo que firmware.
- ADC1 y `analogReadMilliVolts`: convierte señal adaptada del módulo pH a mV; **no** convierte a pH automáticamente.
- DS18B20/1-Wire: temperatura digital; `DallasTemperature`, pull-up y conversión ~750 ms a 12 bits.
- `WA::Readings`: datos; `WA::Decision`: resultado; `WA::StateMachine::evaluate()`: condiciones, histéresis y diagnóstico.
- `WAController::update()`: adquiere y evalúa cada ~1000 ms; `snapshot()`: conserva resultado único para web/LED.
- `WAPhCalibration::fitTwoPoint`: `m=(pH2−pH1)/(V2−V1)`, `b=pH1−mV1`; `estimatePh` solo con modelo válido.
- `WAPhPipeline::Evidence`: `signal_validated` + `calibration_verified`; actualmente **false** en uso real; protege contra falsa validez.
- `WALed::setState`: mezcla R/G/B mediante PWM en ESP32 core 3.x, **solo con cableado confirmado**.
- Campos JSON `mode`, `state`, `calibrated`, `reading_valid`, `sample_age_ms`, `warning`: significado, unidades y límites.
- Los límites 6,5–8,5, margen 0,1, histéresis 0,1 son **referencias provisionales de clasificación**, no norma de potabilidad.

## 9. Escena 8 — Reproducir en **otra placa ESP32 física** (pendiente de hardware)

> **Esta sección es un procedimiento futuro, no un ensayo completado.** Para pasar de otra PC a otra **placa**, confirmar la revisión exacta de hardware, no asumir que el pinout de dos tarjetas ESP32 es idéntico.

1. Comparar módulo Ai-Thinker ESP-32S, revisión PCB UAI, capacidad Flash, reguladores y UART/USB; documentar foto y datasheet.
2. Confirmar pinout y revisar `pins.h`: GPIO de DS18B20, ADC1 pH, tres PWM RGB y pines de strap/reservados; verificar tensión antes de cablear.
3. Instalar driver USB-UART correspondiente al chip real si hace falta y seleccionar el puerto COM correcto; placa/particiones deben corresponder a hardware.
4. Sin sensores, confirmar firmware MOCK y compilar/subir `.ino`; abrir Monitor Serial **115200**.
5. Separadamente cargar la imagen FS mediante **Upload LittleFS** (según herramienta/placa), usando **el mismo esquema** de particiones y Flash real. No sobrescribir datos importantes de otro proyecto.
6. El Monitor Serial indicará SSID y IP de SoftAP. Desde segundo dispositivo conectarse a `WaterAlpha`, entrar en `http://<IP-reportada>/`; abrir `/api/readings`.
7. Verificar sitio completo y JSON `mode:MOCK` antes de tocar sensores.
8. Integrar uno por uno DS18B20, módulo AFEL protegido/ADC1, LED RGB con resistencias; activar flags físicos individualmente y registrar cada gate de prueba.
9. Para pH: calibrar físicamente con patrones, verificar errores y persistencia **antes de permitir GREEN/RED como estado de lectura real**. Mientras no se verifique debe mostrarse MAGENTA.
10. Hacer demostración integrada cronometrada ≥5 minutos y video/log; actualizar CAD/esquemático/README al montaje final.

## 10. Problemas frecuentes y resolución

| Síntoma | Diagnóstico / solución |
|---|---|
| `python` no se reconoce | Revisar instalación, PATH o usar `py -3 ...` si Windows lo permite |
| `ERR_ADDRESS_INVALID` con `[::]:8000` | Abrir `http://127.0.0.1:8000/` y usar `--bind 127.0.0.1` |
| `g++` no se reconoce | Instalar GCC MSYS2 UCRT64 y añadir `C:\msys64\ucrt64\bin` temporalmente al PATH |
| `Anomaly` no es un tipo | Evitar macros Arduino `HIGH`/`LOW`: usar `ABOVE_MAX`/`BELOW_MIN` |
| `LittleFS` no declarado | Revisar `#include <LittleFS.h>` **con almohadilla** y paquete ESP32 instalado |
| `OneWire.h` o `DallasTemperature.h` ausente | Gestor de bibliotecas; nombres/autores correctos; reiniciar IDE si corresponde |
| Página funciona en `8000` pero no en `8001` | Revisar `API_MODE`, servidor Python, carpeta `data`, puerto ocupado y `/api/readings` |
| Activar API REAL en `python -m http.server` da HTTP 404 | Servidor estático **no implementa** `/api/readings`; usar `tests/mock_http_server.py` |
| `/` en ESP32 devuelve archivo no disponible | No basta compilar; montar LittleFS, generar/subir imagen para partición exacta |
| No hay cambio físico de LED | En este estado `USE_REAL_LED=false`; sin hardware eso es intencional |
| Lectura pH sigue MAGENTA en modo REAL | Calibración/evidencia no verificadas; **no** solucionarlo cambiando flags a true artificialmente |
| Arduino «upload failed» | Revisar puerto, alimentación, drivers, BOOT/EN, cable de datos y selección exacta de placa; no forzar ajustes indiscriminados |
| Archivo `.ino` parece vacío | Guardar VS Code, comprobar ruta idéntica y reabrir sketch |
| GitHub Desktop dice `No local changes` | Todo comprometido localmente; confirmar también sincronización remota (`Fetch origin` vs `Push origin`) |

## 11. Guion de grabación propuesto (20–30 min ajustables)

| Minuto orientativo | Mostrar | Resultado observable |
|---|---|---|
| 0:00–1:30 | Alcance y advertencia | Qué hace/no hace WaterAlpha |
| 1:30–4:30 | GitHub Desktop + clone + árbol | Código en máquina nueva |
| 4:30–7:00 | Python + servidor 8000 | UI MOCK y controles |
| 7:00–9:30 | Servidor 8001/API | JSON dinámico vía HTTP |
| 9:30–13:00 | MSYS2/GCC + tests | Consolas de pruebas |
| 13:00–18:00 | Arduino IDE + bibliotecas + ESP32 core | Compilación firmware |
| 18:00–21:00 | Complemento LittleFS + Build | `mklittlefs.bin` generado |
| 21:00–25:00 | Arquitectura, flags, FSM, calibración | Explicación técnica clara |
| 25:00–27:00 | Preparación de placa, riesgos y pendientes | Honestidad sobre hardware físico |

No publicar credenciales personales, URL privada sin autorización, token GitHub, claves definitivas, datos de usuarios ni imágenes de equipo sin consentimiento. Si se comparte el vídeo públicamente, usar repositorio de ejemplo o acceso controlado para material confidencial.

## 12. Lista de auditoría del tutorial

- [ ] El repo fue clonado desde GitHub en lugar de copiar archivos al azar.
- [ ] Se indica commit, fecha, Windows, core ESP32, IDE y versiones de librerías utilizadas.
- [ ] Se distingue terminal raíz vs carpeta `web/`.
- [ ] Se muestran instalaciones y sus verificaciones.
- [ ] Se ejecutan los cinco tests con salidas capturadas.
- [ ] Se compila el `.ino` y no se confunde con carga física.
- [ ] Se construye LittleFS y se explica que exige carga separada al dispositivo.
- [ ] Se explican los tres niveles de MOCK/REAL (navegador, servidor, sensores).
- [ ] Se explican pH, mV, calibración, validación, estados e histéresis.
- [ ] Se registran problemas reales ocurridos y cómo se resolvieron.
- [ ] Toda simulación se rotula como simulación.
- [ ] El tutorial final permite que una segunda PC reproduzca la prueba desde el repo.

## 13. Referencias para instalar/verificar versiones

- GitHub Desktop: https://desktop.github.com/
- VS Code: https://code.visualstudio.com/
- Python: https://www.python.org/downloads/
- Arduino IDE: https://www.arduino.cc/en/software
- Arduino ESP32: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
- MSYS2: https://www.msys2.org/
- LittleFS plugin: https://github.com/earlephilhower/arduino-littlefs-upload
- DallasTemperature: https://github.com/milesburton/Arduino-Temperature-Control-Library
- OneWire: https://github.com/PaulStoffregen/OneWire

**Criterio:** documentación oficial y versiones comprobadas **en la instalación real**, no asumir que URL o interfaz permanece inmutable.  
**Documentación complementaria:** [prototipo](03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md) · [matriz](06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md) · [riesgos](07_REGISTRO_RIESGOS_Y_PENDIENTES.md).
