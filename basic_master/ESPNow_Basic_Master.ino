#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

esp_now_peer_info_t slave;
#define CHANNEL 1

void initializeEspNow() {
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESPNow Init Failed, restarting...");
    ESP.restart();
  }
  Serial.println("ESPNow Init Success");
}

void scanForSlave() {
  bool slaveFound = false;
  memset(&slave, 0, sizeof(slave));

  int16_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL);
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
    return;
  }

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    if (SSID.indexOf("Slave") == 0) {
      Serial.println("Found a Slave.");
      int mac[6];
      if (6 == sscanf(WiFi.BSSIDstr(i).c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
        for (int ii = 0; ii < 6; ++ii) {
          slave.peer_addr[ii] = (uint8_t)mac[ii];
        }
      }
      slave.channel = CHANNEL;
      slave.encrypt = 0;
      slaveFound = true;
      break;
    }
  }

  WiFi.scanDelete();

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }
}

bool manageSlave() {
  if (slave.channel != CHANNEL) {
    Serial.println("No Slave found to process");
    return false;
  }

  bool exists = esp_now_is_peer_exist(slave.peer_addr);
  if (exists) {
    Serial.println("Already Paired");
    return true;
  }

  esp_err_t addStatus = esp_now_add_peer(&slave);
  if (addStatus == ESP_OK) {
    Serial.println("Pair success");
    return true;
  }

  Serial.println("Slave pair failed!");
  return false;
}

void sendData() {
  static uint8_t data = 0;
  data++;
  esp_err_t result = esp_now_send(slave.peer_addr, &data, sizeof(data));
  if (result == ESP_OK) {
    Serial.println("Send Success");
  } else {
    Serial.println("Send Failed");
  }
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.println("ESPNow/Basic/Master Example");
  initializeEspNow();
  esp_now_register_send_cb(onDataSent);
}

void loop() {
  scanForSlave();
  if (slave.channel == CHANNEL && manageSlave()) {
    sendData();
  }
  delay(3000);
}
