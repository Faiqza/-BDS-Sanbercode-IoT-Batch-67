#include <WiFi.h>
#include <MQTT.h>
#include <Arduino_JSON.h>  

// Ganti dengan info jaringan WiFi kamu
const char ssid[] = "CASA Lt2";
const char pass[] = "goknitua";

// Ganti dengan info broker dari shiftr.io milikmu
const char mqttBroker[] = "sarasa.cloud.shiftr.io";
const char mqttUsername[] = "Faiq";      
const char mqttPassword[] = "Sarasa2306";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect() {
  // Menyambungkan ke WiFi
  Serial.print("Menghubungkan ke WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi terhubung");

  // Menyambungkan ke MQTT broker
  Serial.print("Menghubungkan ke MQTT...");
  while (!client.connect("arduinoClient", mqttUsername, mqttPassword)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT terhubung");

  client.subscribe("/data/diri");
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin(mqttBroker, net);
  connect();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  // Kirim data setiap 5 detik
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();

    // Membuat JSON
    JSONVar data;
    data["nama"] = "Faiq";
    data["umur"] = 22;
    data["alamat"] = "Cileungsi";

    String jsonString = JSON.stringify(data);
    client.publish("/data/diri", jsonString);

    Serial.println("JSON terkirim: " + jsonString);
  }
}
