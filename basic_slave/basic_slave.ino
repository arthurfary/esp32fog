#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1
#define LED_PIN 2
#define SSID "Slave_1"
#define PASSWORD "Slave_1_Password"

void initializeEspNow() {
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESPNow Init Failed, restarting...");
    ESP.restart();
  }
  Serial.println("ESPNow Init Success");
}

void configureAccessPoint() {
  if (!WiFi.softAP(SSID, PASSWORD, CHANNEL, 0)) {
    Serial.println("AP Config failed.");
    return;
  }
  Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
}

void blinkLed(int times, int delayTime) {
  for (int i = 0; i < times; i++) {
   digitalWrite(LED_PIN, HIGH);
   delay(delayTime);
   digitalWrite(LED_PIN, LOW);
   delay(delayTime);
  }
}

void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  digitalWrite(LED_PIN, HIGH);
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  Serial.println("");
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Slave Example");
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  configureAccessPoint();
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  initializeEspNow();
  esp_now_register_recv_cb(onDataReceived);
  blinkLed(4, 100);
}

void loop() {
  // Chill
}
