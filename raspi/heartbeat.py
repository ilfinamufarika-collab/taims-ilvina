"""
Heartbeat agent untuk Raspberry Pi.
Jalankan: python3 heartbeat.py
"""
import paho.mqtt.client as mqtt
import json, time, socket
from datetime import datetime, timezone, timedelta

MQTT_HOST = "localhost"
MQTT_PORT = 1883
MQTT_USER = None
MQTT_PASS = None
DEVICE_ID = "IOT-RPI-01"
TOPIC     = f"taims/heartbeat/{DEVICE_ID}"
INTERVAL  = 30

def get_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except:
        return "0.0.0.0"

def on_connect(client, userdata, flags, rc):
    print(f"[MQTT] Connected, rc={rc}")

def on_disconnect(client, userdata, rc):
    print(f"[MQTT] Disconnected, rc={rc}")

client = mqtt.Client(client_id=f"RPI-{DEVICE_ID}")
client.will_set(
    TOPIC,
    '{"device_id":"IOT-RPI-01","status":"connection_lost"}',
    qos=1,
    retain=False
)
if MQTT_USER:
    client.username_pw_set(MQTT_USER, MQTT_PASS)
client.on_connect = on_connect
client.on_disconnect = on_disconnect

lwt = json.dumps({"status": "offline", "reason": "connection_lost"})
client.will_set(TOPIC, lwt, qos=1, retain=False)

boot_time = time.monotonic()
wib = timezone(timedelta(hours=7))

print(f">> Heartbeat RPi untuk {DEVICE_ID}")

client.connect(MQTT_HOST, MQTT_PORT, 60)
client.loop_start()

try:
    while True:
        payload = {
            "status":           "online",
            "timestamp":        datetime.now(wib).isoformat(),
            "ip_address":       get_ip(),
            "uptime_seconds":   int(time.monotonic() - boot_time),
            "firmware_version": "1.0.0"
        }
        client.publish(TOPIC, json.dumps(payload), qos=1)
        print(f"[OK] [{datetime.now().strftime('%H:%M:%S')}] Sent")
        time.sleep(INTERVAL)
except KeyboardInterrupt:
    print("\n>> Dihentikan")
    client.loop_stop()
    client.disconnect()
