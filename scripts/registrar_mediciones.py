#!/usr/bin/env python3
"""WaterAlpha: registro del ESP32 al PC en CSV/JSONL, sin acceso a internet.
Ejemplo: python scripts/registrar_mediciones.py --url http://192.168.4.1/api/readings
En el mismo PC conectado al Wi-Fi del ESP32. Ctrl+C para finalizar.
No escribe en GitHub ni en la Flash del ESP32.
"""
import argparse
import csv
import json
import time
from datetime import datetime, timezone
from pathlib import Path
from urllib.request import Request, urlopen

FIELDS = ["session_id", "iteration", "pc_timestamp_local", "pc_timestamp_utc",
          "mode", "ph", "ph_estimated", "ph_adc_mv", "temperature_c",
          "state", "screening_status", "stable", "calibrated", "reading_valid",
          "ph_sensor_ok", "temperature_sensor_ok", "sample_age_ms",
          "potability", "capture_result", "error"]

def fetch(url, timeout):
    with urlopen(Request(url,headers={"Accept":"application/json","Cache-Control":"no-cache"}),timeout=timeout) as response:
        data=json.loads(response.read(16384).decode("utf-8"))
    if data.get("schema_version") != 1 or data.get("mode") not in ("MOCK", "REAL"):
        raise ValueError("Contrato de /api/readings incompatible")
    return data

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--url",default="http://192.168.4.1/api/readings")
    parser.add_argument("--seconds",type=float,default=2.0)
    parser.add_argument("--count",type=int,default=0,help="0=continuo hasta Ctrl+C")
    parser.add_argument("--allow-mock",action="store_true",help="Solo ensayos, no evidencia fisica")
    args=parser.parse_args()
    if args.seconds<0.5:parser.error("--seconds debe ser al menos 0.5")
    started=datetime.now().astimezone()
    sid=started.strftime("%Y%m%d_%H%M%S_%f")
    folder=Path(__file__).resolve().parents[1]/"data"/"registros"/started.strftime("%Y-%m-%d")
    folder.mkdir(parents=True,exist_ok=True)
    prefix=folder/("WA_"+sid)
    (prefix.with_suffix(".metadata.json")).write_text(json.dumps({
        "session_id":sid,"url":args.url,"started_local":started.isoformat(),
        "clock":"timestamp del PC (no reloj del ESP32)","period_seconds":args.seconds},indent=2),encoding="utf-8")
    mapping={"mode":"mode","ph":"ph","ph_estimated":"ph_estimated",
             "ph_adc_mv":"ph_adc_mv","temperature_c":"temperature", "state":"state",
             "screening_status":"screening_status","stable":"stable",
             "calibrated":"calibrated","reading_valid":"reading_valid",
             "ph_sensor_ok":"ph_sensor_ok","temperature_sensor_ok":"temperature_sensor_ok",
             "sample_age_ms":"sample_age_ms","potability":"potability"}
    n=0
    with prefix.with_suffix(".csv").open("w",newline="",encoding="utf-8") as f, \
         prefix.with_suffix(".jsonl").open("w",encoding="utf-8") as j:
        writer=csv.DictWriter(f,fieldnames=FIELDS);writer.writeheader()
        try:
            while args.count==0 or n<args.count:
                now=datetime.now().astimezone()
                try:
                    data=fetch(args.url,3.0)
                    if data["mode"]=="MOCK" and not args.allow_mock:
                        print("API en MOCK: no registrar como datos reales. Usa --allow-mock solo para ensayo.")
                        break
                    error=""
                except Exception as exc:
                    data=None;error=f"{type(exc).__name__}: {exc}"
                n+=1
                row={k:"" for k in FIELDS}
                row.update(session_id=sid,iteration=n,
                           pc_timestamp_local=now.isoformat(timespec="milliseconds"),
                           pc_timestamp_utc=now.astimezone(timezone.utc).isoformat(timespec="milliseconds"),
                           capture_result="ERROR" if error else ("SIMULATED" if data["mode"]=="MOCK" else "REAL"),
                           error=error)
                if data is not None:
                    for dest,source in mapping.items():
                        value=data.get(source)
                        row[dest]="" if value is None else value
                    if isinstance(data.get("sample_age_ms"),(int,float)) and data["sample_age_ms"]>4000:
                        row["capture_result"]="STALE" if data["mode"]=="REAL" else "SIMULATED_STALE"
                writer.writerow(row);f.flush()
                j.write(json.dumps({"record":row,"raw_api":data},ensure_ascii=False)+"\n");j.flush()
                print(n,row["capture_result"],"pH",row["ph"],"estimado",row["ph_estimated"],
                      "T",row["temperature_c"],"estado",row["state"])
                if args.count==0 or n<args.count:time.sleep(args.seconds)
        except KeyboardInterrupt:
            print("Registro detenido por usuario")
    print("Sesiones guardadas en:",folder)

if __name__=="__main__":
    main()
