#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include <HTTPClient.h>


WiFiClient espClient;
PubSubClient mqtt(espClient);

unsigned long lastBeat = 0;
unsigned long bootTime = 0;

void connectWiFi() {
  Serial.print("Connect WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK: " + WiFi.localIP().toString());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connect MQTT...");
    String clientId = "ESP32-" + String(DEVICE_ID);
    String lwt = "{\"status\":\"offline\",\"reason\":\"connection_lost\"}";
    
    bool ok;
    if (strlen(MQTT_USER) > 0) {
      ok = mqtt.connect(clientId.c_str(), MQTT_USER, MQTT_PASS,
                        TOPIC_HEARTBEAT, 1, false, lwt.c_str());
    } else {
      ok = mqtt.connect(clientId.c_str(), NULL, NULL,
                        TOPIC_HEARTBEAT, 1, false, lwt.c_str());
    }
    
    if (ok) {
      Serial.println(" OK");
    } else {
      Serial.print(" gagal, rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

void kirimHeartbeat() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi putus, reconnect...");
    connectWiFi();
  }
  if (!mqtt.connected()) connectMQTT();

  StaticJsonDocument<300> doc;
  doc["status"]           = "online";
  doc["ip_address"]       = WiFi.localIP().toString();
  doc["rssi"]             = WiFi.RSSI();
  doc["uptime_seconds"]   = (millis() - bootTime) / 1000;
  doc["firmware_version"] = FIRMWARE;

  String payload;
  serializeJson(doc, payload);

  String topic = "taims/heartbeat/" + String(DEVICE_ID);
  bool ok = mqtt.publish(topic.c_str(), payload.c_str(), false);
  Serial.println(ok ? "[OK] Heartbeat terkirim" : "[GAGAL]");
  Serial.println("  " + payload);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  bootTime = millis();

  connectWiFi();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  connectMQTT();
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }

  mqtt.loop();

  if (millis() - lastBeat >= HEARTBEAT_INTERVAL_MS) {
    lastBeat = millis();
    kirimHeartbeat();
  }
}
