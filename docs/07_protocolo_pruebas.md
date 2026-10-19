
# WATERALPHA
## Protocolo de pruebas del prototipo Alpha

### 1. Objetivo

Verificar el funcionamiento de los subsistemas
electrónicos y digitales de WaterAlpha.

El dispositivo realiza una evaluación preliminar.
No certifica potabilidad ni ausencia de patógenos.

### 2. Subsistemas evaluados

- Alimentación de la placa ESP32.
- Conectividad Wi-Fi SoftAP.
- Servidor HTTP e interfaz web.
- Sensor de temperatura DS18B20.
- Adquisición analógica del módulo de pH.
- Calibración y validación de pH.
- Máquina de estados.
- Indicador visual RGB.

### 3. Condiciones previas

- Confirmar modelo y pinout de la placa.
- Verificar tensiones con un multímetro.
- Confirmar el rango de salida del módulo de pH.
- Instalar la adaptación eléctrica necesaria.
- Disponer de soluciones patrón de pH.
- Mantener la electrónica protegida del agua.
- Registrar versiones de firmware y configuración.

### 4. Ensayos

| ID | Ensayo | Resultado esperado |
|---|---|---|
| P01 | Encendido del ESP32 | Arranque sin errores |
| P02 | Red Wi-Fi WaterAlpha | Red visible y conexión exitosa |
| P03 | GET /api/readings | JSON válido y actualizado |
| P04 | Interfaz web local | Datos y LED virtual visibles |
| P05 | DS18B20 en agua | Temperatura estable |
| P06 | Desconexión DS18B20 | Error de lectura identificado |
| P07 | Señal pH en ADC1 | Voltaje adquirido sin saturación |
| P08 | Calibración con patrones | Coeficientes obtenidos y registrados |
| P09 | Verificación independiente de pH | Error cuantificado |
| P10 | Anomalía simulada de pH | Estado RED |
| P11 | Histéresis de pH | Sin oscilación incorrecta |
| P12 | Medición no validada | Estado MAGENTA |
| P13 | LED RGB | Coincide con estado del firmware |
| P14 | Funcionamiento sin Internet | Interfaz accesible localmente |

### 5. Registro de cada ensayo

Para cada ensayo registrar:

- Identificador de prueba:
- Fecha:
- Operador:
- Componentes utilizados:
- Version de firmware:
- Procedimiento realizado:
- Valores observados:
- Resultado esperado:
- Resultado obtenido:
- Evidencia (fotografia, log o captura):
- Estado: PENDIENTE / APROBADO / FALLIDO
- Observaciones:

### 6. Calibración del sensor de pH

Registrar para cada solución patrón:

- pH nominal del patrón.
- Temperatura durante la medición.
- Voltaje ADC en mV.
- Número de mediciones.
- Promedio y dispersión.
- Tiempo de estabilización.

Calcular pendiente e intercepto con los
datos experimentales.

Verificar la estimación con una solución
independiente cuando sea posible.

Registrar error y condiciones de prueba.

### 7. Criterios de aceptación

- Ninguna lectura sin validación debe
  presentarse como medición confiable.
- El estado de la interfaz debe coincidir
  con el estado calculado por el firmware.
- Los errores de sensores deben producir
  un diagnóstico explícito.
- La conexión local debe funcionar
  sin depender de Internet.
- Las tolerancias de exactitud se definirán
  antes de los ensayos físicos.

### 8. Estado actual

Pruebas de software realizadas en computador.

Pruebas eléctricas, calibración física,
mediciones reales y validación integral:
PENDIENTES.
