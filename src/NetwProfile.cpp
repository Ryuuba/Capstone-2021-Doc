#include "NetwProfile.h"

bool NetworkProfile::begin(const char* filename, HardwareSerial& serial) {
  bool success = false;
  if(SPIFFS.begin(true)) {
    File f = SPIFFS.open(filename, "r");
    if (f) {
      uint16_t fileSize = f.size();
      buffer = std::unique_ptr<char[]>(new char[fileSize]);
      f.readBytes(buffer.get(), fileSize);
      DeserializationError error = deserializeJson(doc, buffer.get(), fileSize);
      if(!error) {
        profile = doc["profile"];
        wifi.ssid = doc["WiFi"]["SSID"];
        wifi.passwd = doc["WiFi"]["passwd"];
        mqttConn.id = doc["MQTT"]["id"];
        mqttConn.broker_addr = doc["MQTT"]["broker"];
        mqttConn.port = doc["MQTT"]["port"];
        JsonObject publish = doc["MQTT"]["publish"];
        co2.topic = publish["co2"]["topic"];
        co2.QoS = publish["co2"]["QoS"];
        temp.topic = publish["temperature"]["topic"];
        temp.QoS = publish["temperature"]["QoS"];
        humidity.topic = publish["humidity"]["topic"];
        humidity.QoS = publish["humidity"]["QoS"];
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