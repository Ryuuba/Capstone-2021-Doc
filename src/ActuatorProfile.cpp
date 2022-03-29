#include "ActuatorProfile.h"

bool ActuatorProfile::begin(const char* filename, HardwareSerial& serial) {
  bool success = false;
  if(SPIFFS.begin(true)) {
    File f = SPIFFS.open(filename, "r");
    if (f) {
      uint16_t fileSize = f.size();
      buffer = std::unique_ptr<char[]>(new char[fileSize]);
      f.readBytes(buffer.get(), fileSize);
      DeserializationError error = deserializeJson(doc, buffer.get(), fileSize);
      if (!error)  {
        JsonObject amberLEDProfile = doc["amberLED"];
        amberLED.pin = amberLEDProfile["pin"];
        amberLED.mode = amberLEDProfile["mode"];
        JsonObject redLEDProfile = doc["amberLED"];
        redLED.pin = redLEDProfile["pin"];
        redLED.mode = redLEDProfile["mode"];
        JsonObject fanProfile = doc["amberLED"];
        fan.pin = fanProfile["pin"];
        fan.mode = fanProfile["mode"];
        success = true;
      }
      else {
        serial.println("deserializeJson() fails");
        serial.printf("Error: %s\n", error.c_str());
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