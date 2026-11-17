"""Copia la interfaz fuente al LittleFS del sketch; subir la imagen FS aparte."""
from pathlib import Path
import shutil
root=Path(__file__).resolve().parents[1]
source=root/'web'/'index.html'
target=root/'firmware'/'WaterAlpha'/'data'/'index.html'
target.parent.mkdir(parents=True,exist_ok=True)
shutil.copy2(source,target)
print('Interfaz copiada a:',target)
print('IMPORTANTE: reconstruir y subir LittleFS desde Arduino IDE.')
