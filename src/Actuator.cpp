#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFiSetup.h"
#include "ActuatorProfile.h"
#include "NetwProfile.h"
#include "ConstantDefs.h"

WiFiSetup* wifi;
WiFiClient espClient;
PubSubClient client(espClient);
NetworkProfile nProfile;
ActuatorProfile actProfile;

void readCO2PPM(char* topic, byte* message, unsigned int length) {
  Serial.printf("Receiving reading from %s\n", topic);
  String co2ReadingStr;
  for (uint8_t i = 0; i < length; i++) 
    co2ReadingStr += char(message[i]);
  uint16_t co2Reading = co2ReadingStr.toInt();
  Serial.printf("CO2: %i ppm\n", co2Reading);
  if (co2Reading < 600) {
    digitalWrite(actProfile.amberLED.pin, LOW);
    digitalWrite(actProfile.redLED.pin, LOW);
    digitalWrite(actProfile.fan.pin, LOW);
  }
  else if(co2Reading < 800) {
    digitalWrite(actProfile.amberLED.pin, HIGH);
    digitalWrite(actProfile.redLED.pin, LOW);
    digitalWrite(actProfile.fan.pin, LOW);
  }
  else if(co2Reading < 1000) {
    digitalWrite(actProfile.amberLED.pin, LOW);
    digitalWrite(actProfile.redLED.pin, HIGH);
    digitalWrite(actProfile.fan.pin, LOW);
  }
  else {
    digitalWrite(actProfile.amberLED.pin, LOW);
    digitalWrite(actProfile.redLED.pin, LOW);
    digitalWrite(actProfile.fan.pin, HIGH);
    Serial.println("Warning!!!");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initialization starts");
  if(!actProfile.begin(ACTCONFILE, Serial)) {
    Serial.printf(
      "Actuator profile cannot be opened from file: %s\n", 
      ACTCONFILE
    );
    while(true);
  }
  Serial.println("Actuator profile OK");
  pinMode(actProfile.amberLED.pin, actProfile.amberLED.mode);
  pinMode(actProfile.redLED.pin, actProfile.redLED.mode);
  pinMode(actProfile.fan.pin, actProfile.fan.mode);
  Serial.printf("amberLED: %i, redLED: %i, fan: %i\n", actProfile.amberLED.pin, actProfile.redLED.pin, actProfile.fan.pin);
  if (!nProfile.begin(NETWCONFILE, Serial)) {
    Serial.printf(
      "Network profile cannot be opened from file: %s\n", 
      NETWCONFILE
    );
    while(true);
  }
  Serial.println("Network profile OK");
  wifi = new WiFiSetup(nProfile.wifi.ssid, nProfile.wifi.passwd);
  wifi->begin(Serial);
  client.setServer(nProfile.mqttConn.broker_addr, nProfile.mqttConn.port);
  client.connect(nProfile.mqttConn.id);
  client.subscribe(nProfile.co2.topic);
  client.setCallback(readCO2PPM);
  Serial.println("Initialization OK");
}

void loop() {
  if (!client.connected())
  {
    if (!client.connect(nProfile.mqttConn.id))
      Serial.println("MQTT client: Lost connection to broker");
  }
  else
    client.loop();
}