"""Copia web/ a firmware/WaterAlpha/data/. El sitio local sigue MOCK."""
from pathlib import Path
import shutil

ROOT=Path(__file__).resolve().parents[1]
SRC=ROOT/"web"
DST=ROOT/"firmware"/"WaterAlpha"/"data"
FILES=("index.html","style.css","config.js","mock.js","state_machine.js","api.js","app.js")
def main():
    DST.mkdir(parents=True,exist_ok=True)
    for filename in FILES:
        shutil.copy2(SRC/filename,DST/filename)
    cfg=DST/"config.js"
    data=cfg.read_text(encoding="utf-8")
    if 'API_MODE: "MOCK"' not in data:
        raise ValueError("web/config.js no contiene API_MODE: MOCK")
    cfg.write_text(data.replace('API_MODE: "MOCK"','API_MODE: "REAL"',1),encoding="utf-8")
    print("Datos web sincronizados a LittleFS. Falta SUBIR la imagen LittleFS en Arduino IDE.")
if __name__=="__main__":
    main()
