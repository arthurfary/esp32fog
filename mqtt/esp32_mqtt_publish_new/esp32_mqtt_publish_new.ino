#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_now.h>
#include <bitset>

#define CHANNEL 1
#define LED_PIN 2
#define SSID "Slave_1"
#define PASSWORD "Slave_1_Password"
#define MQTT_SERVER "test.mosquitto.org"
#define MQTT_PORT 1883

uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x78, 0xC8, 0x78};

typedef struct package_type {
  char slave;
  int data;
  int group;
} package_type;

package_type incomingData;
package_type fowardingData;

esp_now_peer_info_t peerInfo;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void blinkLed(int times, int delayTime) {
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < times; i++) {
   digitalWrite(LED_PIN, HIGH);
   delay(delayTime);
   digitalWrite(LED_PIN, LOW);
   delay(delayTime);
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
      
    if (mqttClient.connect(clientId.c_str())) {
      mqttClient.subscribe("/fog/parityInfo");  // subscribe to topic
    }
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Received [" + String(topic) + "]: " + message);
}

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int data_len) {
  String dataStr = String(incomingData.data);

  mqttClient.publish("/fog/parityInfo", dataStr.c_str());
  Serial.println("Data: " + incomingData.data + BIN);
  Serial.println("Slave: " + incomingData.slave);

  blinkLed(1, 10);
}

void setup() {
  blinkLed(8, 50);
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);

  
}

void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
}