#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define CHANNEL 1
#define NUMSLAVES 3
esp_now_peer_info_t slaves[NUMSLAVES] = {};
int SlaveCnt = 0;
#define PRINTSCANRESULTS 0

void initializeEspNow() {
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESPNow Init Failed, restarting...");
    ESP.restart();
  }
  Serial.println("ESPNow Init Success");
}

void scanForSlave() {
 // bool slaveFound = false;
  memset(slaves, 0, sizeof(slaves));
  SlaveCnt = 0;
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
          slaves[SlaveCnt].peer_addr[ii] = (uint8_t)mac[ii];
        }
      }
      slaves[SlaveCnt].channel = CHANNEL;
      slaves[SlaveCnt].encrypt = 0;
      //slaveFound = true;
      SlaveCnt++;
    }
  }

  if (SlaveCnt > 0) {
    Serial.print(SlaveCnt); Serial.println("Slaves Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }
  WiFi.scanDelete();
}

void manageSlave() {
  if (SlaveCnt > 0) {
    for (int i = 0; i < SlaveCnt; i++) {
      Serial.print("Processing: ");
      for (int ii = 0; ii < 6; ++ii ) {
        Serial.print((uint8_t) slaves[i].peer_addr[ii], HEX);
        if (ii != 5) Serial.print(":");
      }
      Serial.print(" Status: ");

      bool exists = esp_now_is_peer_exist(slaves[i].peer_addr);
      if (exists) {
        Serial.println("Already Paired");
      } else {
        esp_err_t addStatus = esp_now_add_peer(&slaves[i]);
        if (addStatus == ESP_OK) {
          Serial.println("Pair success");
        } else {
          Serial.println("Pair failed");
        }
        delay(100);
      }
    }
  } else {
    Serial.println("No Slave found to process");
  }
}

static uint16_t data = 0b1010101010100010; // Dado binário de 16 bits

void sendData() {
  uint8_t firstPart = data >> 8; // Pega os 8 bits mais significativos
  uint8_t secondPart = data & 0xFF; // Pega os 8 bits menos significativos
  esp_err_t result;

  for (int i = 0; i < SlaveCnt; i++) {
    const uint8_t *peer_addr = slaves[i].peer_addr;
    if (i == 0) {
      Serial.print("Sending: ");
      Serial.println(data, BIN);
      esp_err_t result = esp_now_send(peer_addr, &firstPart, sizeof(firstPart));
      Serial.println(firstPart, BIN);
    }
    else if (i == 1){
      esp_err_t result = esp_now_send(peer_addr, &secondPart, sizeof(secondPart));
      Serial.println(secondPart, BIN);
    }
    else{
      uint8_t Xor = firstPart ^ secondPart;
      esp_err_t result = esp_now_send(peer_addr, &Xor, sizeof(Xor));
      Serial.println(Xor, BIN);
    }
    // if (result == ESP_OK) {
    //   Serial.println("Send Success");
    // } else {
    //   Serial.println("Send Failed");
    // }
  }
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success " : "Delivery Fail ");
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);

  if (status != ESP_NOW_SEND_SUCCESS) {
    for (int i = 0; i < SlaveCnt; i++) {
      if (memcmp(slaves[i].peer_addr, mac_addr, 6) == 0) {
        esp_now_del_peer(slaves[i].peer_addr);
        for (int j = i; j < SlaveCnt - 1; j++) {
          memcpy(&slaves[j], &slaves[j + 1], sizeof(esp_now_peer_info_t));
        }
        SlaveCnt--;
        Serial.println("Slave removed due to delivery fail");
        break;
      }
    }
  }
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
  if (SlaveCnt == 3){
    sendData();
  }
  else{
    scanForSlave();
    manageSlave();
  }
  delay(1000);
}
