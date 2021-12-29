#include <Arduino.h>
#include <Wire.h>
#include "ConstantDefs.h"
#include "CO2Sensor.h"

CO2Sensor cm1107;
bool testLedStatus;

void setup()
{
  Wire.begin(SDA, SCL, 10000);
  Serial.begin(9600);
  cm1107.begin(Wire, CM1107_MODEL);
  cm1107.printSerialNumber(Serial);
  cm1107.printSoftwareVersion(Serial);
  pinMode(TEST_LED, OUTPUT);    // Digital pin blinking a LED
  testLedStatus = false;
  delay(5000);
}

void loop()
{
  cm1107.read();
  cm1107.printDataFrame(Serial);
  cm1107.printCO2Measurement(Serial);
  cm1107.printStatus(Serial);
  testLedStatus = !testLedStatus;
  digitalWrite(TEST_LED, testLedStatus);
  delay(3000);
}