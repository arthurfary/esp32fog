#include <esp_now.h>
#include <WiFi.h>
#include <bitset>

#define CHANNEL 1
#define LED_PIN 2
#define SSID "Slave_1"
#define PASSWORD "Slave_1_Password"

uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x78, 0xC8, 0x78};

typedef struct package_type {
  char slave;
  int data;
  int group;
} package_type;

package_type incomingData;

package_type fowardingData;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    Serial.println("Delivery Success :)");
  }
  else{
    Serial.println("Delivery Fail :(");
  }
  blinkLed(2, 10);
}

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

void sendIncomingData(package_type fowardingData){
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &fowardingData, sizeof(fowardingData));

  if (result == ESP_OK){
    Serial.println(":)");
  }
  else{
    Serial.println(":(");
  }
}

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int data_len) {
  // recieved package
  memcpy(&incomingData, data, sizeof(incomingData));
  
  digitalWrite(LED_PIN, HIGH);
  // Use info->src_addr to access the MAC address
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           info->src_addr[0], info->src_addr[1], info->src_addr[2], 
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println(incomingData.data, BIN);
  Serial.print("Last Packet Recv Data: "); Serial.println(incomingData.slave);
  Serial.println("");
  fowardingData = incomingData;
  sendIncomingData(fowardingData);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Slave Example");
  pinMode(LED_PIN, OUTPUT);
  WiFi.mode(WIFI_STA); //WiFi.mode(WIFI_AP);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;


  configureAccessPoint();
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  initializeEspNow();

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    Serial.println(esp_now_add_peer(&peerInfo));
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  blinkLed(4, 20);
}

void loop() {
  // Chill
}
