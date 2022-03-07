#if !defined(SENSOR_PROFILE_H)
#define SENSOR_PROFILE_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>

/**
 * @brief Loads the sensor profile from a JSON file
 * 
 */
class SensorProfile {
public:
  uint32_t i2cFreq;
  struct {
    uint8_t address;
    uint8_t height;
    uint8_t width;
    uint8_t rst_pin;
    uint32_t freq;
  } oled;
  struct {
    uint8_t address;
    const char* model;
    uint16_t baseline;
  } co2Sensor;
  struct {
    uint8_t pin;
    uint8_t mode;
  } button;
  struct {
    uint8_t pin;
    uint8_t mode;
  } led;
  /**
   * @brief Constructs a new Sensor Profile object from a JSON file stored in
   * the ESP 32 file system
   * @note Compute the member class size using the assistant from 
   * https://arduinojson.org/v6/assistant/
   */
  SensorProfile() { }
  /**
   * @brief Destroys the Sensor Profile object
   * 
   */
  ~SensorProfile() { }
  /**
   * @brief Parses a JSON file to initialize all variables of structures oled, \
   * co2Sensor, button, led
   * @param filename The name of the JSON configuration file
   * @returns true if the JSON file is correctly parsed, otherwise, false
   */
  bool begin(const char*, HardwareSerial&);
private:
  // @brief The size of the JSON doc data structure
  static const uint16_t size = 512;
  StaticJsonDocument<size> doc;
  std::unique_ptr<char[]> buffer;
};

#endif // SENSOR_PROFILE_H
