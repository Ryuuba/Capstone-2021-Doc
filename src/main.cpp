#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h>
#include "ConstantDefs.h"
#include "CO2Sensor.h"
#include "WiFiSetup.h"

// Global objects
CO2Sensor cm1107;
bool testLedStatus;
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1, OLED_FREQ, BASE_FREQ);
WiFiSetup wifi(SSID_HOME, PASSWD_HOME);
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Wire.begin(SDA, SCL, BASE_FREQ);
  Serial.begin(9600);
  if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, false))
  {
    Serial.println("Oled allocation failed");
    while (true);
  }
  cm1107.begin(Wire, CM1107_MODEL);
  cm1107.printSerialNumber(Serial);
  cm1107.printSoftwareVersion(Serial);
  wifi.begin(Serial);
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.connect(CLIENT_ID);
  pinMode(TEST_LED, OUTPUT);    // Digital pin blinking a LED
  testLedStatus = false;
  delay(50000);
}

void loop()
{
  uint16_t CO2 = cm1107.read();
  cm1107.printDataFrame(Serial);
  cm1107.printCO2Measurement(Serial);
  cm1107.printStatus(Serial);
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0, 0);
  oled.printf("CO2 sensor status\n");
  oled.setTextSize(1);
  oled.printf("Frame: %s\n", cm1107.getPrintableDataFrame().c_str());
  oled.printf("CO2 measurement: %i\n", CO2);
  oled.printf("Status: %s\n", cm1107.getStatus().c_str());
  oled.display();
  if (!client.connected())
  {
    if (!client.connect(CLIENT_ID))
      Serial.println("MQTT client: Lost connection to broker");
  }
  else
  {
    client.loop();
    client.publish(CO2_TOPIC, cm1107.getCO2().c_str());
  }
  testLedStatus = !testLedStatus;
  digitalWrite(TEST_LED, testLedStatus);
  delay(5000);
}

