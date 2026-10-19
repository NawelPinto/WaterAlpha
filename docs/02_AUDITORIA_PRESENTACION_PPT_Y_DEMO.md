# Auditoría especializada — PPT y demostración oral WaterAlpha


## 1. Qué piden los documentos y qué no dicen

Especifica **5–7 minutos de demostración presencial + 2 de feedback**, participación de todos los integrantes, **funcionamiento sensor–actuador–conectividad, estabilidad integrada ≥5 minutos**, CAD de encapsulado, testing/iteración y documentación. Una tabla asigna: funcionalidad 36 %, integración 21 %, encapsulado 14 %, iteración 14 %, documentación 7 % y comunicación 7 % (suma 99 % por redondeo o inconsistencia de pauta)



## 2. Mensaje rector de la defensa

> «WaterAlpha es un dispositivo portátil orientado a senderistas para una **evaluación preliminar** de parámetros del agua. En Alpha priorizamos pH y temperatura; el ESP32 adquiere/valida datos, ejecuta una máquina de estados y representa la decisión en LED RGB y una interfaz web local sin Internet. Hoy mostramos [**decir exactamente: simulación, pruebas de código, o integración física**]; no certificamos potabilidad ni ausencia de patógenos».


## 3. Guion recomendado: seis láminas máximas

1 **Problema y alcance**: 1 imagen/silueta de usuario + pregunta, no párrafos

2 **Solución y arquitectura**:  Diagrama muestra → justificar el uso de ESTOS sensores, mostrar flujo de funcionamiento.

3 **Diseño electrónico y decisiones**:  Qué está implementado y qué requiere verificar al llegar hardware.

4 **CAD Fusion 360**: vista isométrica + corte explotado/acotado, dimensiones reales,  Separación zona húmeda/seca, accesos, mantenimiento, compatibilidad volumétrica.

5 **Pruebas, iteraciones y límites**: Tabla mínima de resultados reales/compilados + captura de FSM y bugs resueltos. Qué se ensayó, criterios, evidencia, qué *no* se pudo ensayar.

6  **Demostración / conclusión**: Página WaterAlpha abierta o transición a demo; solo 3 conclusiones | Cambios de estado observables, coherencia JSON (Datos sueltos en un archivo json)/LED, siguiente riesgo crítico.

**Total orientativo:** 5:10–5:50 de narrativa y transiciones, sujeto al modo de demo. La tabla no reemplaza la obligación de mostrar **operación continua ≥5 minutos** exigida por Avance #2: esa condición se verifica con **sesión cronometrada independiente** (puede arrancar antes de iniciar la exposición y quedar registrada), y debe confirmarse el formato permitido con el docente. Si se requiere observar los cinco minutos íntegros dentro de la exposición, reorganizar tiempos/diapositivas.

### Contenido detallado por diapositiva

**1. Problema:** una frase sobre brecha de acceso a información preliminar, usuario objetivo senderista. Si se citan cifras OMS/Chile del documento teórico, copiar referencia original verificable, año, población y definición de indicador antes de utilizarlas. No dedicar media presentación a la Entrega 1.

**2. Ingeniería:** incluir cadena de adaptación de niveles, interruptores/flags de seguridad y estado del pinout.

**3. CAD:** usar render propio de Fusion 360, no una geometría sugerida por el estudio como si ya estuviera comprobada. Indicar alto/diámetro/espesores medidos en CAD, pasos de cable, sonda, volumen para PCB/batería y cómo se limpia la cámara.

**4. Evidencia:** contraste de pruebas unitarias vs integración real: Escoger una iteración que efectivamente haya cambiado desempeño observable.



## 4. Guion operativo de demo con ESP32 


1. Verificar energía, pinout, adaptación AFEL, cableado, resistencias LED y protección seco/húmedo **antes de encender**.
2. Cargar firmware y **por separado** la imagen LittleFS en partición verificada; abrir Monitor Serial 115200 para registrar IP anunciada.
3. Conectar el celular/notebook al SSID `WaterAlpha`, usando credenciales del equipo; aclarar que la red **no ofrece Internet**.
4. Abrir `http://<IP-anunciada-por-el-ESP32>/` (p. ej. `192.168.4.1` solo si fue la reportada).
5. Demostrar temperatura y pH reales en condiciones controladas; mostrar estado simultáneo del LED y de `GET /api/readings`.
6. Introducir anomalía controlada  y retorno con histéresis; desconectar un sensor solo cuando sea seguro y preparado.
7. Registrar ≥5 minutos continuos y anomalías/errores, con hora y confirmación de conectividad. *No mantener agua cerca de electrónica expuesta*.

## 5. Plan A, contingencias y roles

| Fallo | Recuperación permitida | Cómo comunicarlo sin engañar |
|---|---|---|
| Wi-Fi no conecta | Revisar red, clave, distancia, IP serial; reiniciar bajo protocolo
| LittleFS no carga | Verificar esquema y upload FS; conservar video/documentación de última prueba
| pH inestable | Volver a solución patrón y verificar calibración/ruido |
| Sensor desconectado | Activar diagnóstico error; comprobar cableado desenergizado cuando corresponda |
| Imposibilidad de hardware | Abrir demo MOCK local y explicitar alcance | 


## 6. Preguntas técnicas probables y respuestas cortas

**¿Por qué ESP32?** Wi-Fi integrado y capacidad de ejecutar adquisición, estado, actuador y servidor HTTP sin router ni nube.  
**¿Por qué ADC1?** En ESP32 clásico ADC2 presenta conflicto de uso con Wi-Fi; verificar el GPIO concreto disponible en UAI.  
**¿Por qué cinco colores?** Separan inicialización, sin anomalía, precaución, fuera de referencia y error de adquisición/calibración.  
**¿Qué es histéresis?** Memoria que exige una recuperación suficiente antes de abandonar RED; evita oscilaciones por ruido cerca de umbral.  
**¿Cuál es el criterio de pH?** Referencia preliminar de diseño 6,5–8,5, margen 0,1 y histéresis 0,1; no certificación sanitaria.  
**¿Qué prueba el website actual?** Presentación de respuestas JSON en HTTP con datos simulados; la prueba en placa está pendiente.  
**¿Qué mide hoy el prototipo físico?** **Nada validado físicamente** mientras no se conecten y calibren los sensores; jamás insinuar otra cosa.


