# WaterAlpha — Índice y guía de uso de las auditorías

**Universidad Adolfo Ibáñez · TEI201 — Taller de Diseño en Ingeniería**  
**Fecha de corte documental:** 18 de octubre de 2026  
**Carácter:** auditoría técnico-académica de preparación, no certificación de laboratorio ni aprobación docente.  
**Responsable de actualización:** [Sofía Kunihiro, Simon *apellido*, Florencia Venegas, Anibal Pinto]  
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

Se analizaron los materiales suministrados al proyecto:
- `Instructivo_Entrega_2_Informe_APA.docx`, 
- `Plantilla_Entrega_2_APA.docx`, 
-`avance_02_estudiantes.pdf`,
- `Base teórica y técnica para la Entrega 2_ dispositivo portátil de evaluación preliminar de agua para.pdf`,
- `Clase_TEI201_Sistemas_Electronicos_Control.pptx`, 
- `TEI201_Clase_5_Diseno_CAD_Fusion360.pptx` 


### Clasificación de estados

- **V — Verificado por evidencia accesible:** instructivo o archivo leído, o resultado técnico concreto comunicado en la conversación y localizado por nombre. Cuando es una prueba comunicada por el usuario, anotar *V-reportado*, no *V-independiente*.
- **D — Desarrollado / reportado:** código, modelo o funcionalidad descrita y compilada según comunicación, pero sin revisión directa del artefacto final.
- **P — Pendiente:** no implementado o falta evidencia de aceptación.
- **B — Bloqueado:** necesita hardware, datos, acceso o decisión todavía no disponible.
- **R — Riesgo/observación:** aspecto que puede invalidar la afirmación si no se corrige.

## Condición final de cierre

No marcar «prototipo IoT físico funcional» hasta obtener, en **hardware real**, evidencia de lectura de sensores, actuación del LED, enlace Wi-Fi local, datos actualizados, calibración, pruebas de falla y operación integrada durante **≥5 minutos**, según la pauta de Avance #2. El funcionamiento del servidor Python por sí solo no cumple ese criterio.
