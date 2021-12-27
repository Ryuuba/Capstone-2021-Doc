#include <Arduino.h>
#include <Wire.h>
#define TEST_LED 25

bool test_led_status;

bool checksum(uint8_t data[], int len) {
  uint16_t accum = 0;
  for (size_t i = 0; i < len - 1; ++i) 
      accum += data[i];
  uint8_t carry = (accum & 0xFF00) >> 8;
  uint8_t cs = (0x00FF & accum) + carry;
  cs += data[len - 1];
  return cs == 0;
}

void setup() {
  Wire.begin(21, 22, 10000);
  Wire.beginTransmission(0x31);
  Wire.write(byte(0x01));
  Wire.endTransmission();
  Serial.begin(9600);
  pinMode(TEST_LED, OUTPUT);    // Digital pin blinking a LED
  test_led_status = false;
  delay(500);
}

void loop() {
  uint8_t measurement_frame[5];
  Wire.requestFrom(0x31, 5);
  if (Wire.available() == 5)
  {
    measurement_frame[0] = Wire.read();
    measurement_frame[1] = Wire.read();
    measurement_frame[2] = Wire.read();
    measurement_frame[3] = Wire.read();
    measurement_frame[4] = Wire.read();
    Serial.print("Data frame: "); 
    for (byte i = 0; i < 5; i++)
      Serial.printf("%x ", measurement_frame[i]);
    Serial.println();
    if (checksum(measurement_frame, 5))
    {
      if (measurement_frame[3] == 0)
      {
        uint16_t co2 = (measurement_frame[1] << 8) + measurement_frame[2];
        Serial.printf("CO2 measurement: %i\n", co2);
      }
      else
        Serial.printf("Status issue: %i\n", measurement_frame[3]);
    }
    else
      Serial.printf("Checksum fails\n");
  }
  else
  {
    Serial.printf("Incorrect measurement\n");
  }
  test_led_status = !test_led_status;
  digitalWrite(TEST_LED, test_led_status);
  delay(3000);
}