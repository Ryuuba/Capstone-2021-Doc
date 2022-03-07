#include "SensorProfile.h"

bool SensorProfile::begin(const char* filename, HardwareSerial& serial) {
  bool success = false;
  if(SPIFFS.begin(true)) {
    File f = SPIFFS.open(filename, "r");
    if (f) {
      uint16_t fileSize = f.size();
      buffer = std::unique_ptr<char[]>(new char[fileSize]);
      f.readBytes(buffer.get(), fileSize);
      DeserializationError error = deserializeJson(doc, buffer.get(), fileSize);
      if(!error) {
        JsonObject i2c = doc["i2c"];
        i2cFreq = i2c["freq"];
        JsonObject i2c_oled = i2c["oled"];
        oled.address = i2c_oled["address"];
        oled.height = i2c_oled["height"];
        oled.width = i2c_oled["width"];
        oled.rst_pin = i2c_oled["rstPin"];
        oled.freq = i2c_oled["freq"];
        JsonObject i2c_CO2sensor = i2c["CO2sensor"];
        co2Sensor.address = i2c_CO2sensor["address"];
        co2Sensor.model = i2c_CO2sensor["model"];
        co2Sensor.baseline = i2c_CO2sensor["baseline"];
        JsonObject sensor_led = doc["led"];
        led.pin = sensor_led["pin"];
        led.mode = sensor_led["mode"];
        JsonObject sensor_button = doc["button"];
        button.pin = sensor_button["pin"]; // 32
        button.mode = sensor_button["mode"]; // 5
        f.close();
        success = true;
      }
      else {
        serial.println("deserializeJson() fails");
      }
    }
    else {
      serial.println("SPIFFS.open() fails");
    }
  }
  else {
    serial.println("SPIFFS.begin() fails");
  }
  return success;
}