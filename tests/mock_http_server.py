
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from time import monotonic
import json

# Carpeta de archivos web destinados al ESP32
ROOT = Path(__file__).resolve().parents[1]
WEB_DIR = ROOT / "firmware" / "WaterAlpha" / "data"

START_TIME = monotonic()

WARNING = (
    "Evaluación preliminar. No certifica potabilidad "
    "ni ausencia de patógenos."
)

# Escenarios: pH, estado, estabilidad, sensor conectado
SCENARIOS = [
    (7.20, "GREEN", True, True),
    (8.45, "YELLOW", True, True),
    (8.55, "RED", True, True),
    (8.45, "RED", True, True),
    (8.40, "YELLOW", True, True),
    (7.20, "GREEN", True, True),
    (5.80, "RED", True, True),
    (6.55, "RED", True, True),
    (6.60, "YELLOW", True, True),
    (7.20, "GREEN", True, True),
    (7.20, "MAGENTA", True, False),
    (7.20, "YELLOW", False, True),
]

MESSAGES = {
    "BLUE": "Esperando mediciones válidas.",
    "GREEN": "pH dentro de referencia.",
    "YELLOW": "Precaución: pH cercano al límite o lectura inestable.",
    "RED": "pH fuera de referencia.",
    "MAGENTA": "Sensor o calibración inválida.",
}


class WaterAlphaHandler(SimpleHTTPRequestHandler):

    def __init__(self, *args, **kwargs):
        super().__init__(
            *args,
            directory=str(WEB_DIR),
            **kwargs
        )

    def do_GET(self):

        path = self.path.split("?", 1)[0]

        # Otros archivos: HTML, CSS y JavaScript
        if path != "/api/readings":
            return super().do_GET()

        elapsed = monotonic() - START_TIME
        initializing = elapsed < 3

        if initializing:
            ph = 7.20
            state = "BLUE"
            stable = True
            ph_ok = True
        else:
            index = int((elapsed - 3) // 5) % len(SCENARIOS)
            ph, state, stable, ph_ok = SCENARIOS[index]

        data = {
            "schema_version": 1,
            "mode": "MOCK",
            "temperature": 17.8,
            "ph": ph if ph_ok else None,
            "state": state,
            "stable": stable,
            "temperature_sensor_ok": True,
            "ph_sensor_ok": ph_ok,
            "calibrated": True,
            "reading_valid": (
                not initializing and stable and ph_ok
            ),
            "sample_age_ms": 0,
            "message": MESSAGES[state],
            "warning": WARNING
        }

        body = json.dumps(
            data,
            ensure_ascii=False
        ).encode("utf-8")

        self.send_response(200)
        self.send_header(
            "Content-Type",
            "application/json; charset=utf-8"
        )
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()

        self.wfile.write(body)


if __name__ == "__main__":

    if not WEB_DIR.is_dir():
        raise SystemExit(
            f"No existe la carpeta web: {WEB_DIR}"
        )

    server = ThreadingHTTPServer(
        ("127.0.0.1", 8001),
        WaterAlphaHandler
    )

    print("WaterAlpha - Servidor de pruebas HTTP")
    print("http://127.0.0.1:8001/")
    print("Detener con Ctrl+C")

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
