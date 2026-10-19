# Auditoría general — WaterAlpha (TEI201, UAI)

**Fecha de corte:** 18-10-2026 · **Naturaleza:** diagnóstico de madurez, brechas y conformidad académica.  
**Alcance:** diseño, CAD, electrónica, software, metrología, testing, demostración, paper, reproducibilidad.  


## 1. Criterios de ingeniería y decisiones tomadas

- **Hardware previsto actualizado:** placa personalizada UAI con módulo clásico Ai-Thinker ESP-32S; no sustituir por FireBeetle sugerida en la base investigativa.
- **Sensores Alpha:** temperatura DS18B20 y pH AFEL. Conductividad y turbidez pertenecen a alcance futuro; CAD puede reservar espacio, pero no decir que hoy miden.
- **ADC1 + Wi-Fi:** planificar pH en entrada ADC1 efectivamente accesible; no asignar GPIO hasta revisar pinout UAI.
- **5 V vs 3,3 V:** pH AFEL alimentado según ficha real y salida adaptada; nunca aplicar 5 V sin comprobar a un GPIO.
- **DS18B20:** alimentación propuesta 3,3 V y pull-up ~4,7 kΩ entre DQ y 3V3; verificar colores y variante antes de cablear.
- **Máquina de estados:** límites pH 6,5–8,5 son provisionales; margen 0,1 y liberación por histéresis 0,1; no confundir con regulación sanitaria.
- **LED RGB:** cinco colores, posible ánodo/cátodo común; tres resistencias limitadoras calculadas según ficha y GPIO; cableado pendiente.
- **Alimentación/carcasa:** USB 5 V para primeras pruebas y separación húmedo/seco. IP67, masa objetivo y autonomía son hipótesis hasta ensayos.

## 2. Evaluación del método de prueba

**Logros comunicados:** visualización web local MOCK, simulación de controles, compilación Arduino, instalación MSYS2 `g++`, pruebas unitarias de estados, pruebas HTTP desde servidor Python, generación de imagen LittleFS y tests sintéticos de calibración/pipeline. El resultado literal `WaterAlpha: pruebas OK` fue confirmado expresamente para la FSM; otros casos deben acompañarse con salida de consola conservada en evidencias.

**No está demostrado aún:** medición real de pH, exactitud, estabilidad del módulo AFEL, calibración con patrones, temperatura real, consumo, autonomía, estanqueidad, conectividad SoftAP real, LED físico, ≥5 minutos de operación integrada.



## 4. Recomendaciones ordenadas

**Fase A, sin hardware:** comprobar tests con valores predeterminados,documentar lista de dependencias, preparar PPT e informe con estado real, 
**Fase B, al recibir hardware:** verificar referencia/Flash/pinout; alimentación aislada y niveles; DS18B20; salida AFEL sin conectar al ADC antes de adaptar; LED; firmware/FS; pruebas por subsistema.  
**Fase C, integración:** soluciones patrón, calibración, validación independiente, fallos, captura de logs 5 min, vídeo integral, actualización de diagramas y manuscrito.  

## 11. Puertas de aprobación (gates)

- **G0 — Software preparado:** pruebas reproducibles, Arduino compila, contrato web, LittleFS generado. **Estado: alcanzado según reportes; pendiente auditoría directa del commit.**
- **G1 — Electrical safe:** ficha y pinout UAI, mediciones de voltaje, ADC protegido, corriente LED limitada. **Pendiente/bloqueado.**
- **G2 — Sensores funcionando:** datos con fuente real identificada, errores explícitos y calibración trazable. **Pendiente/bloqueado.**
- **G3 — Prototipo Alpha integrado:** LED físico, SoftAP y web real durante ≥5 min sin fallos relevantes. **Pendiente/bloqueado.**
- **G4 — Entrega documental:** PPT, informe APA con evidencia, CAD revisado y anexos. **Pendiente.**
