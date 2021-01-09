/*
   use this to connect you ESP32 with test.mosquitto.org and
   node-red dashboard.some changes may require if used ina ESP8266

   @author:hasithakms@gmail.com
*/

#include "Wire.h"
#include "WiFi.h"
#include <PubSubClient.h>

int recno = 0;
char recno_str[8];

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "";
const char* password = "";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more outputs)
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  Wire.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    //example publish
    client.publish("esp3223rr324564t456g/test/stat", "Connected");
    Serial.print("connected ");
  }

  dtostrf( recno, 1, 2, recno_str);
  client.publish("esp3223rr324564t456g/stat/recno", recno_str);
  Serial.print(recno);
  recno++;

   Serial.println("");

  delay(1000);
  client.loop();
}
