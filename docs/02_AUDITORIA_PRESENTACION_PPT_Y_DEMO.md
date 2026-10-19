# Auditoría especializada — PPT y demostración oral WaterAlpha

**Destinatarios:** equipo expositor TEI201, Universidad Adolfo Ibáñez.  
**Estado:** propuesta de guion y pauta; **el PPT definitivo no fue recibido ni auditado**.  
**Fecha de corte:** 18-10-2026.  
**Cruce:** [Auditoría general](01_AUDITORIA_GENERAL_WATERALPHA.md) · [Matriz de evidencias](06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md).

## 1. Qué piden los documentos y qué no dicen

**Fuente contrastada:** `avance_02_estudiantes.pdf`, pp. 2–8. Corresponde al **Avance #2: Prototipo Alpha IoT Funcional**, cuya fecha consignada es **12-10-2026**. Especifica **5–7 minutos de demostración presencial + 2 de feedback**, participación de todos los integrantes, **funcionamiento sensor–actuador–conectividad, estabilidad integrada ≥5 minutos**, CAD de encapsulado, testing/iteración y documentación. Una tabla asigna: funcionalidad 36 %, integración 21 %, encapsulado 14 %, iteración 14 %, documentación 7 % y comunicación 7 % (suma 99 % por redondeo o inconsistencia de pauta; no alterar cifras originales).

**No se encontró en los adjuntos una pauta independiente que imponga exactamente X diapositivas para el PPT de Entrega 2.** La solicitud de presentar en PPT es reportada por el equipo. Recomendación editorial: **5–6 diapositivas principales** y material de respaldo sin exponer, porque la demostración en vivo tiene prioridad. Confirmar con el docente si las fechas, duración y ponderaciones de Avance #2 también se aplican a la presentación formal de Entrega 2.

**Diferencia crítica:** la presentación oral no se satisface mostrando únicamente un website con respuestas de Python. Esa es evidencia de una simulación / frontend, **no de lectura física y actuación real**. Si no llega hardware, informar la restricción y pedir aceptación de la modalidad de simulación técnicamente justificada (el instructivo del *paper* la contempla como contingencia, mientras la guía *Avance #2* es explícita al exigir funcionamiento real).

## 2. Mensaje rector de la defensa

> «WaterAlpha es un dispositivo portátil orientado a senderistas para una **evaluación preliminar** de parámetros del agua. En Alpha priorizamos pH y temperatura; el ESP32 adquiere/valida datos, ejecuta una máquina de estados y representa la decisión en LED RGB y una interfaz web local sin Internet. Hoy mostramos [**decir exactamente: simulación, pruebas de código, o integración física**]; no certificamos potabilidad ni ausencia de patógenos».

**Evitar:** «detecta todas las impurezas», «agua segura», «OMS aprueba el agua», «precisión certificada de 0,1 pH», «IP67 comprobado», «sensor conectado» si no lo está. Sustituir por «sin anomalía detectada en los parámetros medidos y validados».

## 3. Guion recomendado: seis láminas máximas

| Nº | Título sugerido | Evidencia/visual principal | Mensaje oral | Tiempo orientativo |
|---|---|---|---|---|
| 1 | **Problema y alcance** | 1 imagen/silueta de usuario + pregunta, no párrafos | Senderistas; apariencia del agua insuficiente; resultado *preliminar*, no potabilidad | 0:35 |
| 2 | **Solución y arquitectura** | Diagrama muestra → DS18B20/AFEL → ESP32 → FSM → LED + Wi-Fi local/web | Por qué sensores, ESP32 y SoftAP; futuro EC/NTU no implementado | 0:55 |
| 3 | **Diseño electrónico y decisiones** | Esquemático simplificado, evidencia de ADC1, 3,3/5 V, protección, GPIO pendientes | Qué está implementado y qué requiere verificar al llegar hardware | 0:50 |
| 4 | **CAD Fusion 360** | 1 vista isométrica + corte explotado/acotado, dimensiones reales de **su** archivo | Separación zona húmeda/seca, accesos, mantenimiento, compatibilidad volumétrica | 0:50 |
| 5 | **Pruebas, iteraciones y límites** | Tabla mínima de resultados reales/compilados + captura de FSM y bugs resueltos | Qué se ensayó, criterios, evidencia, qué *no* se pudo ensayar | 1:00 |
| 6 | **Demostración / conclusión** | Página WaterAlpha abierta o transición a demo; solo 3 conclusiones | Cambios de estado observables, coherencia JSON/LED, siguiente riesgo crítico | 1:00–1:40 |

**Total orientativo:** 5:10–5:50 de narrativa y transiciones, sujeto al modo de demo. La tabla no reemplaza la obligación de mostrar **operación continua ≥5 minutos** exigida por Avance #2: esa condición se verifica con **sesión cronometrada independiente** (puede arrancar antes de iniciar la exposición y quedar registrada), y debe confirmarse el formato permitido con el docente. Si se requiere observar los cinco minutos íntegros dentro de la exposición, reorganizar tiempos/diapositivas; **no prometer cumplir simultáneamente una demo de 5 min y 6 min de narración**.

### Contenido detallado por diapositiva

**1. Problema:** una frase sobre brecha de acceso a información preliminar, usuario objetivo senderista; contextualizar SSR rurales solo si se mantiene como audiencia secundaria y existen antecedentes del equipo. Si se citan cifras OMS/Chile del documento teórico, copiar referencia original verificable, año, población y definición de indicador antes de utilizarlas. No dedicar media presentación a la Entrega 1.

**2. Arquitectura:** rotular `DS18B20 °C`, `AFEL pH mV (calibración pendiente)`, `ADC1`, `WAController`, `WA::StateMachine`, `LED RGB`, `SoftAP HTTP`, `GET /api/readings`; trazar energía 5 V/3,3 V por separado. Si la demostración continúa en MOCK, marcar en la figura **«componentes físicos previstos / simulación actual»**.

**3. Ingeniería:** incluir cadena de adaptación de niveles, interruptores/flags de seguridad y estado del pinout. La placa UAI ESP-32S es elección actualizada; la investigación que menciona FireBeetle/DFRobot es solo antecedente.

**4. CAD:** usar render propio de Fusion 360, no una geometría sugerida por el estudio como si ya estuviera comprobada. Indicar alto/diámetro/espesores medidos en CAD, pasos de cable, sonda, volumen para PCB/batería y cómo se limpia la cámara. No afirmar resistencia/IP67 sin ensayo.

**5. Evidencia:** contraste de pruebas unitarias vs integración real: `test_state_machine`, `test_ph_calibration`, `test_ph_pipeline`, `test_system_flow`, `test_led_colors` (solo marcar aprobadas las que tengan salida guardada), compilación Arduino, servidor Python HTTP, build LittleFS; bitácora de errores `HIGH/LOW` y `#include <LittleFS.h>` con antes/después. Escoger una iteración que efectivamente haya cambiado desempeño observable.

**6. Demo:** mostrar primera pantalla real, diagnóstico visible, JSON, pH/T, y transición de LED virtual y físico **solo si físicamente implementado**. Si permanece en MOCK, declarar origen y explicar de forma explícita lo que todavía se simula. Cerrar con una limitación prioritaria y siguiente ensayo.

## 4. Guion operativo de demo: dos modalidades

### A. Demo local sin ESP32 (válida como evidencia de software, no como IoT físico)

1. En VS Code verificar `firmware/WaterAlpha/data/config.js` con `API_MODE: "REAL"`; **la fuente en `web/config.js` puede permanecer en MOCK**.
2. Abrir terminal en raíz del repo, ejecutar `python tests/mock_http_server.py`.
3. Abrir navegador en `http://127.0.0.1:8001/` (misma computadora; no usar la IP del ESP32).
4. Mostrar LED virtual, mediciones y panel «Datos recibidos de la API (JSON)»; observar GREEN → YELLOW → RED, histéresis, MAGENTA en sensor simulado desconectado.
5. Mostrar el contrato `mode: "MOCK"`, `schema_version: 1`, `state`, `ph`, `temperature`, `reading_valid`, `sample_age_ms`.
6. Explicar que el servidor Python está **imitando** `/api/readings`; no es firmware en placa, ni medición física de agua.
7. Capturar pantalla o vídeo con fecha, commit y descripción de la condición simulada.

### B. Demo final con ESP32 (aún pendiente)

1. Verificar energía, pinout, adaptación AFEL, cableado, resistencias LED y protección seco/húmedo **antes de encender**.
2. Cargar firmware y **por separado** la imagen LittleFS en partición verificada; abrir Monitor Serial 115200 para registrar IP anunciada.
3. Conectar el celular/notebook al SSID `WaterAlpha`, usando credenciales del equipo; aclarar que la red **no ofrece Internet**.
4. Abrir `http://<IP-anunciada-por-el-ESP32>/` (p. ej. `192.168.4.1` solo si fue la reportada).
5. Demostrar temperatura y pH reales en condiciones controladas; mostrar estado simultáneo del LED y de `GET /api/readings`.
6. Introducir anomalía controlada **sin ingerir la muestra** y retorno con histéresis; desconectar un sensor solo cuando sea seguro y preparado.
7. Registrar ≥5 minutos continuos y anomalías/errores, con hora y confirmación de conectividad. No mantener agua cerca de electrónica expuesta.

## 5. Plan A, contingencias y roles

| Fallo | Recuperación permitida | Cómo comunicarlo sin engañar |
|---|---|---|
| Wi-Fi no conecta | Revisar red, clave, distancia, IP serial; reiniciar bajo protocolo | Mostrar log e indicar fallo de conectividad |
| LittleFS no carga | Verificar esquema y upload FS; conservar video/documentación de última prueba | No decir que web se ejecutó en placa si se usa Python |
| pH inestable | Volver a solución patrón y verificar calibración/ruido | Estado YELLOW o MAGENTA; no manipular coeficientes para «verse bien» |
| Sensor desconectado | Activar diagnóstico error; comprobar cableado desenergizado cuando corresponda | Fallo detectado es evidencia de robustez si se documenta |
| Imposibilidad de hardware | Abrir demo MOCK local y explicitar alcance | Acredita software; no satisface por sí misma funcionalidad física |
| PowerPoint o video falla | PDF de respaldo, screenshots con fecha, capturas del CAD | Diferenciar evidencia grabada de ejecución en vivo |

Asignar responsables nominales: **[Integrante A: contexto/arquitectura]**, **[B: demostración software/hardware]**, **[C: CAD, pruebas y cierre]**. Todos deben poder explicar por qué el pH carece de validación física actual y qué significa MAGENTA. Practicar tres veces, guardar el entorno listo antes de exponer y cronometrar transiciones.

## 6. Preguntas técnicas probables y respuestas cortas

**¿Por qué ESP32?** Wi-Fi integrado y capacidad de ejecutar adquisición, estado, actuador y servidor HTTP sin router ni nube.  
**¿Por qué ADC1?** En ESP32 clásico ADC2 presenta conflicto de uso con Wi-Fi; verificar el GPIO concreto disponible en UAI.  
**¿Por qué cinco colores?** Separan inicialización, sin anomalía, precaución, fuera de referencia y error de adquisición/calibración.  
**¿Qué es histéresis?** Memoria que exige una recuperación suficiente antes de abandonar RED; evita oscilaciones por ruido cerca de umbral.  
**¿Cuál es el criterio de pH?** Referencia preliminar de diseño 6,5–8,5, margen 0,1 y histéresis 0,1; no certificación sanitaria.  
**¿Qué prueba el website actual?** Presentación de respuestas JSON en HTTP con datos simulados; la prueba en placa está pendiente.  
**¿Qué mide hoy el prototipo físico?** **Nada validado físicamente** mientras no se conecten y calibren los sensores; jamás insinuar otra cosa.

## 7. Checklist de salida

- [ ] Confirmar pauta de PPT, duración y fecha vigentes con docente; no asumir que Avance #2 = presentación Entrega 2.
- [ ] Crear 5–6 diapositivas con tipografía grande, 1 idea por slide, fuentes discretas pero completas.
- [ ] Incorporar CAD propio real (vistas y cotas), no render genérico.
- [ ] Incorporar esquemático actualizado con pines **solo si confirmados**.
- [ ] Tener evidencia fechada y referenciada de todos los tests mencionados.
- [ ] Asegurar distinción MOCK / REAL en cada captura y frase.
- [ ] Verificar conexión web y actuation real en placa si está disponible.
- [ ] Registrar ≥5 min de funcionamiento real cuando corresponda al Avance #2.
- [ ] Ensayar intervención de todos y contingencia sin Internet.
- [ ] Cerrar con alcance real, brechas técnicas y disclaimer de potabilidad.

## 8. Fuentes usadas

- `avance_02_estudiantes.pdf`: pp. 2–8; material central para demo, CAD, testing, participación y rúbrica.
- `Instructivo_Entrega_2_Informe_APA.docx`: §1, §3, §8; necesidad de resultados demostrables, arquitectura y reportabilidad.
- `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx`: parámetros y geometría verificable.
- `Clase_TEI201_Sistemas_Electronicos_Control.pptx`: cadena sensor–adquisición–decisión–actuación y restricciones.
- Historial técnico WaterAlpha: módulos y estado actual según equipo.
