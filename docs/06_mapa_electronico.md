
# WaterAlpha — Mapa electrónico preliminar

## Placa principal
- Modelo: placa UAI con Ai-Thinker ESP-32S
- Pinout: pendiente de confirmar
- Alimentación inicial: USB 5 V

## Sensor de temperatura
- Modelo: DS18B20 waterproof
- Alimentación prevista: 3,3 V
- Protocolo: 1-Wire
- GPIO: pendiente
- Pull-up: 4,7 kΩ entre DATA y 3,3 V

## Sensor de pH
- Proveedor: AFEL
- Modelo exacto: pendiente de verificar
- Alimentación nominal: aproximadamente 5 V
- Salida: analógica
- Entrada ESP32: ADC1 pendiente
- Adaptación de tensión: por dimensionar
- Calibración: pendiente

## LED RGB
- Tipo: pendiente de selección
- Pines R, G, B: pendientes
- Resistencias limitadoras: por dimensionar

## Seguridad y comprobaciones
- Verificar voltajes antes de conectar.
- Confirmar disponibilidad real de 3V3 y 5V.
- Establecer tierra común para circuitos
  compatibles con el diseño.
- No aplicar 5 V directamente a un GPIO ESP32.
- Mantener electrónica y conexiones secas.
- No afirmar potabilidad a partir del resultado.


## ¿Qué necesitamos confirmar antes de cablear?
- Fotografía frontal y posterior de placa UAI	(Identificar conectores y componentes).
- Pinout o esquema de la placa	(Determinar GPIO realmente disponibles).
- Modelo exacto del módulo de pH AFEL	(Comprobar alimentación y salida)
- Modelo y tipo de LED RGB	(Diseñar las resistencias y conexiones)
- Fuente de alimentación disponible	(Verificar tensiones y corriente suficiente) *como el microcomponente funciona via usb hacia la red electrica de la universidad lo mas probable es que este paso no sea necesario*