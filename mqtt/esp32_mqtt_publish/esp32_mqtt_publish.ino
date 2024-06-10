//https://espblockly.com/mqtt.html

#include <WiFi.h>
#include <PubSubClient.h>

const char *SSI_D = "d1ng0";
const char *PWD = "12345687";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
const char *mqttServer = "test.mosquitto.org";
int mqttPort = 1883;

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        //mqttClient.subscribe("someTopic");
      }
      
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}


void setup() {
  Serial.begin(115200);
  Serial.print("Connectiog to ");
 
  WiFi.begin(SSI_D, PWD);
  Serial.println(SSI_D);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
     }
  Serial.print("Connected.");

  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
  

}

void loop() {
   if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();

  if(digitalRead(15)){
    mqttClient.publish("/fog/parityInfo", "101010101010101");
  }
  delay(500); 
}