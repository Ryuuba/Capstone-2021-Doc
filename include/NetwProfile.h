#if !defined(NETWORK_PROFILE_H)
#define NETWORK_PROFILE_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>

class NetworkProfile {
private:
  // @brief The size of the configuration file
  static const uint16_t size = 384;
  std::unique_ptr<char[]> buffer;
  StaticJsonDocument<size> doc;
  struct mqttTopic{
    const char* topic;
    uint8_t QoS;
  };
  struct WiFiData
  {
    const char* ssid;
    const char* passwd;
  };
  struct MQTTConn{
    const char* id;
    const char* broker_addr;
    uint16_t port;
  };
public:
  const char* profile;
  MQTTConn mqttConn;
  WiFiData wifi;
  mqttTopic co2;
  mqttTopic humidity;
  mqttTopic temp;
  NetworkProfile()  { }
  ~NetworkProfile() { }
  bool begin(const char*, HardwareSerial&);
};

#endif // NETWORK_PROFILE_H
