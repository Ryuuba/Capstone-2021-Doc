#include <Arduino.h>
#include<ESPmDNS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include <Adafruit_AHTX0.h>
#include "CO2Sensor.h"
#include "WiFiSetup.h"
#include "SensorProfile.h"
#include "NetwProfile.h"
#include "ConstantDefs.h"

// Global objects
CO2Sensor* cm1107;
Adafruit_SSD1306* oled;
WiFiSetup* wifi;
WiFiClient espClient;
PubSubClient client(espClient);
NetworkProfile nProfile;

// Global variables
uint8_t button;
uint8_t led;
bool ledStatus;
unsigned long seconds = 1000L; //Notice the L 
unsigned long minutes;
Adafruit_AHTX0 aht;

void setup()
{
  Serial.begin(9600);
  SensorProfile sProfile;
  Wire.begin(SDA, SCL, sProfile.i2cFreq);
  if (!sProfile.begin(SENSORCONFILE, Serial)) {
    Serial.printf(
      "Sensor profile cannot be opened from file: %s\n", 
      SENSORCONFILE
    );
    while(true);
  }
  oled = new Adafruit_SSD1306(
    sProfile.oled.width, 
    sProfile.oled.height, 
    &Wire, 
    sProfile.oled.rst_pin, 
    sProfile.oled.freq, 
    sProfile.i2cFreq
  );
  if(!oled->begin(SSD1306_SWITCHCAPVCC, sProfile.oled.address, false)) {
    Serial.println("Oled allocation failed");
    while (true);
  }
  cm1107 = new CO2Sensor();
  cm1107->begin(Wire, sProfile.co2Sensor.model);
  cm1107->printSerialNumber(Serial);
  cm1107->printSoftwareVersion(Serial);
  if (aht.begin())
    Serial.println("Found AHT20");
  else
    Serial.println("AHT20 couldn't be connected");
  button = sProfile.button.pin;
  pinMode(button, sProfile.button.mode);
  if (digitalRead(button) == LOW) {
    Serial.println("Entering to calibration mode");
    if (cm1107->calibrateSensor(sProfile.co2Sensor.baseline, button))
      Serial.printf("Set baseline at %i PPM\n", sProfile.co2Sensor.baseline);
  }
  led = sProfile.led.pin;
  pinMode(led, sProfile.led.mode);
  if (!nProfile.begin(NETWCONFILE, Serial)) {
    Serial.printf(
      "Network profile cannot be opened from file: %s\n", 
      NETWCONFILE
    );
    while(true);
  }
  wifi = new WiFiSetup(nProfile.wifi.ssid, nProfile.wifi.passwd);
  wifi->begin(Serial);;
  mdns_init ();// Add
  IPAddress ipaddr = MDNS.queryHost(nProfile.mqttConn.broker_addr);// .local omitted
  client.setServer(ipaddr, nProfile.mqttConn.port);
  client.connect(nProfile.mqttConn.id);
  minutes = sProfile.delay * seconds;
}

void loop()
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  Serial.printf("Temperature: %0.1f C degrees\n", temp.temperature);
  Serial.printf("Hum: %0.01f RH\n", humidity.relative_humidity);
  uint16_t CO2 = cm1107->read();
  cm1107->printDataFrame(Serial);
  cm1107->printCO2Measurement(Serial);
  cm1107->printStatus(Serial);
  oled->clearDisplay();
  oled->setTextSize(2);
  oled->setTextColor(SSD1306_WHITE);
  oled->setCursor(0, 0);
  oled->printf("UAMote\n");
  oled->setTextSize(1);
  //oled->printf("Frame: %s\n", cm1107->getPrintableDataFrame().c_str());
  oled->printf("CO2 measurement: %i\n", CO2);
  oled->printf("Temperature: %0.1f C\n", temp.temperature);
  oled->printf("Hum: %0.01f RH\n", humidity.relative_humidity);
  //oled->printf("Status: %s\n", cm1107->getStatus().c_str());
  oled->display();
  if (!client.connected())
  {
    if (!client.connect(nProfile.mqttConn.id))
      Serial.println("MQTT client: Lost connection to broker");
  }
  else
  {
    client.loop();
    client.publish(nProfile.co2.topic, cm1107->getCO2().c_str());
  }
  ledStatus = !ledStatus;
  digitalWrite(led, ledStatus);
  delay(minutes);
}



