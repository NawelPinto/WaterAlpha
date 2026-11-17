# WaterAlpha — esquema funcional preliminar (NO es un esquemático eléctrico final)

**Estado:** en espera del pinout oficial de la placa UAI Ai-Thinker ESP-32S y de las medidas eléctricas de los componentes reales. Los GPIO `25` y `34` aparecían escritos en un `pins.h` entregado, junto a `PINOUT_CONFIRMED=true`; **esto NO prueba que estén disponibles en los conectores físicos**. En el parche vuelven a `-1` y `false` por seguridad. No conectar basándose en etiquetas hipotéticas.

## Conexión prevista después de comprobar la placa

```text
[PLACA UAI 3V3] --+-------------------- DS18B20 VDD
                   +-- [4,7 kΩ] --+----- DS18B20 DATA
                                  |
[GPIO 1-Wire confirmado] ---------+
[PLACA GND] ----------------------------- DS18B20 GND

[PLACA 5V validada] ---------------------- AFEL VCC (solo si corresponde)
[PLACA GND] ----------------------------- AFEL GND (comun)
[AFEL AO] ------ [R superior] ----+------ GPIO ADC1 confirmado
                                 |
                             [R inferior]
                                 |
[PLACA GND] ---------------------+

[GPIO rojo] ------- [330 Ω]* ---- LED rojo   ---> GND
[GPIO verde] ------ [330 Ω]* ---- LED verde  ---> GND
[GPIO amarillo] --- [330 Ω]* ---- LED amarillo ---> GND

* Resistencia por LED, valores orientativos; comprobar Vf/corriente.
  La polaridad mostrada supone LED activo alto (cátodo a GND).
```

## Cálculo del divisor de tensión — SOLO EJEMPLO, NO AUTORIZA CABLEADO

En un módulo con salida de 0 a +5 V *comprobada* y baja impedancia de fuente, si `R_superior=22,5 kΩ` y `R_inferior=10 kΩ`, entonces:

`V_ADC = V_AO × 10 / (22,5 + 10) ≈ 0,3077 × V_AO`, por lo que **5,0 V se convertirían aproximadamente en 1,54 V**.

**Esto no confirma que el AFEL entregue una señal 0–5 V, que sea positiva en toda condición ni que el divisor no altere la medición.** Medir AO con multímetro primero, medir ADC después del divisor, observar variación de señal con buffers y calibrar los milivoltios **en el punto de entrada ADC**. Si AO es negativo, si hay voltaje fuera de límites, saturación o impedancia inadecuada, detener y rediseñar acondicionamiento. No usar 3,3 V como límite de exactitud del ADC; revisar el rango recomendado por Espressif para la atenuación seleccionada. ADC2 puede quedar indisponible durante Wi-Fi; preferir **ADC1** accesible.

## LED independientes, NO LED RGB

| Estado lógico API | Patrón físico con 3 LED | Qué representa |
|---|---|---|
| `GREEN` | Verde fijo | pH en referencia y señales consideradas estables/validadas |
| `YELLOW` | Amarillo fijo | Precaución: cercano a límite o inestabilidad temporal |
| `RED` | Rojo fijo | pH fuera de referencia experimental |
| `BLUE` | Amarillo intermitente lento (500 ms encendido/500 apagado) | Inicio |
| `MAGENTA` | Rojo intermitente rápido (250 ms encendido/250 apagado) | Falla/no validación |

`led_rgb.cpp` conserva su nombre de archivo por compatibilidad, pero el nuevo controlador maneja tres LED separados. `led_colors.cpp` se mantiene como lógica histórica de RGB y sus tests **no verifican** el semáforo físico; para el nuevo comportamiento hay `tests/test_led_status_logic.cpp`.

**Verificaciones previas para encender luces:** confirmar cada LED con multímetro/comprobador, identificar ánodo/cátodo, confirmar tres resistencias de 220–470 Ω típicas y calcular la corriente prevista, medir voltaje y revisar corriente máxima del GPIO UAI. La hipótesis de 330 Ω por LED sirve como punto de partida para cálculo, no como autorización sin verificar hardware.

## Riesgos y ausencias

- Las 8 unidades de cables Dupont declaradas pueden no alcanzar para dos sensores, alimentación, divisor y tres LED, aun usando rieles comunes. Preparar más puentes M-M y M-F según conectores reales.
- Las dos protoboards necesitan conexiones a GND y VCC verificadas; no conectar rieles de +5 V y +3,3 V entre sí.
- El USB alimenta la placa, pero **no garantiza** que la PCB exponga 5 V y 3V3 adecuados ni suficiente corriente para periféricos. La red eléctrica de la universidad no sustituye la comprobación de fuentes.
- La pieza impresa fotografiada tiene una abertura lateral amplia y perforaciones. Puede servir de **soporte/ensayo geométrico**, pero no demuestra estanqueidad; los conductores y PCB deben quedar secos y separarse de la muestra.
- Este documento debe convertirse en **esquemático real con símbolos, GPIO, terminales, fuentes, resistencia y fotos multímetro** cuando el modelo de placa sea confirmado. La pauta del Avance 3 solicita ese esquema completo.