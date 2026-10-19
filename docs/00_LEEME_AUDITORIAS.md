# WaterAlpha — Índice y guía de uso de las auditorías

**Universidad Adolfo Ibáñez · TEI201 — Taller de Diseño en Ingeniería**  
**Fecha de corte documental:** 18 de octubre de 2026  
**Carácter:** auditoría técnico-académica de preparación, no certificación de laboratorio ni aprobación docente.  
**Responsable de actualización:** [equipo / integrante]  
**Versión del paquete:** 1.0

## Qué contienen estos archivos

| Archivo | Público y finalidad | Pregunta que responde |
|---|---|---|
| `01_AUDITORIA_GENERAL_WATERALPHA.md` | Equipo completo y tutor | ¿Dónde está realmente el proyecto y cuáles son los bloqueos? |
| `02_AUDITORIA_PRESENTACION_PPT_Y_DEMO.md` | Expositores | ¿Qué mostrar en pocas láminas y cómo demostrarlo en vivo? |
| `03_AUDITORIA_PROTOTIPO_HARDWARE_SOFTWARE.md` | Responsables de implementación | ¿Cómo construir, cablear, calibrar, programar y probar el prototipo? |
| `04_AUDITORIA_INFORME_ENTREGA2_APA.md` | Responsables de redacción | ¿Cómo cumplir exactamente el instructivo y sustentar cada afirmación? |
| `05_TUTORIAL_REPLICACION_ENTORNO_Y_SOFTWARE.md` | Autor del video tutorial y futuros equipos | ¿Cómo reproducir el desarrollo desde una segunda computadora/dispositivo? |
| `06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md` | Auditoría y equipo | ¿Qué requisito se apoya en cuál evidencia y qué falta producir? |
| `07_REGISTRO_RIESGOS_Y_PENDIENTES.md` | Líder técnico/equipo | ¿Cuáles son los riesgos, responsables y puertas de aceptación? |

## Alcance y principio de evidencia

Se analizaron los materiales suministrados al proyecto: `Instructivo_Entrega_2_Informe_APA.docx`, `Plantilla_Entrega_2_APA.docx`, `avance_02_estudiantes.pdf`, `Base teórica y técnica para la Entrega 2_ dispositivo portátil de evaluación preliminar de agua para.pdf`, `Clase_TEI201_Sistemas_Electronicos_Control.pptx`, `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx` y el historial técnico de desarrollo de WaterAlpha descrito en esta conversación, incluido `prompt_wateralpha_software_interfaz_web(1).txt`.

**No se inspeccionó el contenido actual del repositorio remoto GitHub, las carpetas reales de VS Code, el CAD nativo Fusion 360, fotografías del circuito ni una placa ESP32 física.** Por tanto, una prueba declarada por el equipo se consigna como *reportada*, no como verificación independiente del código del repositorio. El modelo Fusion 360 se considera **declarado existente por el equipo**, no auditado dimensionalmente.

### Clasificación de estados

- **V — Verificado por evidencia accesible:** instructivo o archivo leído, o resultado técnico concreto comunicado en la conversación y localizado por nombre. Cuando es una prueba comunicada por el usuario, anotar *V-reportado*, no *V-independiente*.
- **D — Desarrollado / reportado:** código, modelo o funcionalidad descrita y compilada según comunicación, pero sin revisión directa del artefacto final.
- **P — Pendiente:** no implementado o falta evidencia de aceptación.
- **B — Bloqueado:** necesita hardware, datos, acceso o decisión todavía no disponible.
- **R — Riesgo/observación:** aspecto que puede invalidar la afirmación si no se corrige.

### Terminología obligatoria

El producto es un **dispositivo portátil de evaluación preliminar de agua**. No es un medidor que certifique potabilidad. El estado **GREEN** significa *sin anomalía detectada en las variables efectivamente medidas y verificadas*, no *agua potable/segura*. Incluir de forma visible: **«Evaluación preliminar. No certifica potabilidad ni ausencia de patógenos.»** Los límites de pH 6,5–8,5 del software son **referencias provisionales de diseño**, no una certificación normativa ni prueba de ausencia de contaminantes.

## Fuentes y jerarquía

1. **Instructivo oficial del informe**: manda sobre formato, estructura, anexos y calidad del paper de Entrega 2.
2. **Plantilla de informe APA**: manda sobre orden y casillas de contenido a completar.
3. **Guía Avance #2 — Prototipo Alpha IoT Funcional**: pauta verificable de la demostración presencial, con fecha especificada **12-10-2026** y duración de **5–7 minutos + 2 minutos de feedback**. **No asumir sin confirmación docente que sea también la pauta formal de un PPT distinto de Entrega 2.** Se reutilizan sus criterios como preparación técnica y oral.
4. **Clases de electrónica/control y CAD Fusion 360**: criterios metodológicos (sensor–adquisición–control–actuación, tolerancias, dimensiones, ensamblaje), no evidencia de desempeño propio.
5. **Base teórica/técnica**: investigación y propuestas orientativas; contiene alternativas DFRobot/FireBeetle e hipótesis geométricas que **no equivalen a compras ni decisiones confirmadas**. Las decisiones actualizadas del equipo prevalecen: placa UAI con ESP-32S, módulo pH AFEL, DS18B20, LED RGB, web local.
6. **Historial del desarrollo**: contratos, estructura de archivos y pruebas comunicadas. Verificar contra el `commit` real antes de la entrega.

## Instrucciones para integrar en VS Code y GitHub

1. Copiar estos **ocho archivos `.md`** a `<repositorio-WaterAlpha>/docs/` sin eliminar `06_mapa_electronico.md` ni `07_protocolo_pruebas.md` ya existentes.
2. En VS Code abrir `00_LEEME_AUDITORIAS.md`, comprobar enlaces relativos entre documentos y revisar campos `[POR COMPLETAR]`.
3. Actualizar `06_MATRIZ_TRAZABILIDAD_Y_EVIDENCIAS.md` después de cada prueba; registrar archivo de log, foto, captura, fecha y commit.
4. Registrar correcciones y decisiones en `07_REGISTRO_RIESGOS_Y_PENDIENTES.md`.
5. En GitHub Desktop revisar `Changes` y crear un commit, por ejemplo: `docs: auditar entregables TEI201 y preparar replicacion WaterAlpha`.
6. No incluir capturas con claves, tokens, datos personales, identificadores sensibles ni accesos privados expuestos.

## Orden de lectura recomendado

- **Para priorizar:** 01 → 07 → 06.
- **Para preparar la defensa:** 02 → 06 → 03.
- **Para escribir el paper:** 04 → 06 → 01.
- **Para grabar el videotutorial:** 05 → 03.

## Condición final de cierre

No marcar «prototipo IoT físico funcional» hasta obtener, en **hardware real**, evidencia de lectura de sensores, actuación del LED, enlace Wi-Fi local, datos actualizados, calibración, pruebas de falla y operación integrada durante **≥5 minutos**, según la pauta de Avance #2. El funcionamiento del servidor Python por sí solo no cumple ese criterio.
