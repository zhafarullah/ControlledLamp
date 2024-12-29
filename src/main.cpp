#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "WifiAku";           // Ganti dengan nama WiFi Anda
const char* password = "aafahmad15";    // Ganti dengan password WiFi Anda

// MQTT Broker
const char* mqtt_broker = "20.2.168.202"; // Ganti dengan IP broker MQTT di Azure
const int mqtt_port = 1883;              // Port default MQTT
const char* mqtt_topic = "hand_pose/data"; // Topik MQTT yang akan di-subscribe

// Pin untuk relay
const int relayPin = 5; 

WiFiClient espClient;
PubSubClient client(espClient);

// Callback ketika pesan diterima
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message += (char)payload[i];
  }
  Serial.println();

  // Kontrol relay berdasarkan pesan
  if (message == "hifive(ON)") {
    digitalWrite(relayPin, HIGH); // Relay ON
    Serial.println("Relay ON: hifive detected.");
  } else if (message == "fist (OFF)") {
    digitalWrite(relayPin, LOW);  // Relay OFF
    Serial.println("Relay OFF: fist detected.");
  } else if (message == "none(Nothing)") {
    Serial.println("No action for none."); // Tidak melakukan apa-apa
  }
}

// Fungsi untuk menghubungkan ke WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  // Atur pin relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Pastikan relay mati di awal

  // Sambungkan ke WiFi
  setup_wifi();

  // Atur server MQTT dan callback
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  // Hubungkan ke broker MQTT
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Connected to MQTT Broker.");
    } else {
      Serial.print("Failed to connect. Error state: ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  // Subscribe ke topik
  client.subscribe(mqtt_topic);
}

void loop() {
  client.loop(); // Pastikan MQTT tetap berjalan
}
