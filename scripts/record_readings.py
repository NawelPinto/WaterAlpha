"""Registro CSV+JSONL de /api/readings de WaterAlpha con fecha de PC e iteracion.
Ejemplo: python scripts/record_readings.py --url http://192.168.4.1/api/readings
Los datos MOCK se rechazan salvo --allow-mock; no se suben solos a GitHub.
"""
import argparse
import csv
import json
import time
from datetime import datetime, timezone
from pathlib import Path
from urllib.request import Request, urlopen

FIELDS=("run_id","iteration","pc_time_local","pc_time_utc","capture_status",
        "source_mode","ph","temperature_c","state","reading_valid","stable",
        "ph_sensor_ok","temperature_sensor_ok","calibrated","sample_age_ms",
        "message","warning","error")

def get_data(url, timeout):
    req=Request(url,headers={"Accept":"application/json","Cache-Control":"no-cache"})
    with urlopen(req,timeout=timeout) as response:
        payload=json.loads(response.read(16384).decode("utf-8"))
    if not isinstance(payload,dict) or payload.get("schema_version") != 1:
        raise ValueError("La API no tiene schema_version=1")
    if payload.get("mode") not in ("MOCK","REAL"):
        raise ValueError("La API no indica origen MOCK/REAL")
    return payload

def main():
    p=argparse.ArgumentParser(description="WaterAlpha: adquisicion persistente desde el PC")
    p.add_argument("--url",default="http://192.168.4.1/api/readings")
    p.add_argument("--interval",type=float,default=2.0)
    p.add_argument("--timeout",type=float,default=3.0)
    p.add_argument("--max-samples",type=int,default=0,help="0: continuo hasta Ctrl+C")
    p.add_argument("--allow-mock",action="store_true")
    p.add_argument("--output",type=Path,default=Path(__file__).resolve().parents[1]/"data"/"registros")
    args=p.parse_args()
    if args.interval<0.25: p.error("--interval debe ser >=0.25 segundos")
    start=datetime.now().astimezone()
    run_id=start.strftime("%Y%m%d_%H%M%S_%f")
    folder=args.output/start.strftime("%Y-%m-%d")
    folder.mkdir(parents=True,exist_ok=True)
    basename=folder/("wateralpha_"+run_id)
    csvfile=basename.with_suffix(".csv")
    jsonfile=basename.with_suffix(".jsonl")
    metafile=basename.with_suffix(".meta.json")
    metafile.write_text(json.dumps({"run_id":run_id,"url":args.url,
        "timestamp_origin":"reloj del PC","started_at":start.isoformat(),
        "interval_seconds":args.interval,"allow_mock":args.allow_mock},
        indent=2,ensure_ascii=False)+"\n",encoding="utf-8")
    print("CSV:",csvfile,"\nJSONL:",jsonfile)
    iteration=0
    with csvfile.open("w",newline="",encoding="utf-8") as f, jsonfile.open("w",encoding="utf-8") as j:
        writer=csv.DictWriter(f,fieldnames=FIELDS)
        writer.writeheader()
        try:
            while not args.max_samples or iteration<args.max_samples:
                error="";payload=None
                try:
                    payload=get_data(args.url,args.timeout)
                except Exception as exc:
                    error=type(exc).__name__+": "+str(exc)
                if payload and payload["mode"]=="MOCK" and not args.allow_mock:
                    print("API en modo MOCK: no se graban muestras sintéticas. Usar --allow-mock para ensayos.")
                    break
                now=datetime.now().astimezone()
                iteration+=1
                row=dict.fromkeys(FIELDS,"")
                row.update(run_id=run_id,iteration=iteration,
                    pc_time_local=now.isoformat(timespec="milliseconds"),
                    pc_time_utc=now.astimezone(timezone.utc).isoformat(timespec="milliseconds"),
                    capture_status="HTTP_ERROR" if error else ("SIMULATED" if payload and payload["mode"]=="MOCK" else "RECORDED"),
                    error=error)
                mapping={"source_mode":"mode","ph":"ph","temperature_c":"temperature","state":"state",
                    "reading_valid":"reading_valid","stable":"stable","ph_sensor_ok":"ph_sensor_ok",
                    "temperature_sensor_ok":"temperature_sensor_ok","calibrated":"calibrated",
                    "sample_age_ms":"sample_age_ms","message":"message","warning":"warning"}
                if payload:
                    for col,key in mapping.items():
                        val=payload.get(key)
                        row[col]="" if val is None else val
                    if isinstance(payload.get("sample_age_ms"),(int,float)) and payload["sample_age_ms"]>5000:
                        row["capture_status"]="STALE" if payload["mode"]=="REAL" else "SIMULATED_STALE"
                writer.writerow(row)
                j.write(json.dumps({"record":row,"raw_api":payload},ensure_ascii=False)+"\n")
                f.flush();j.flush()
                print("#%04d %s pH=%s T=%s mode=%s state=%s"%(iteration,row["pc_time_local"],
                    row["ph"],row["temperature_c"],row["source_mode"],row["state"]))
                if args.max_samples==0 or iteration<args.max_samples:time.sleep(args.interval)
        except KeyboardInterrupt:
            print("Registro terminado, archivos guardados.")
    print("Muestras/eventos registrados:",iteration)
if __name__=="__main__":
    main()
