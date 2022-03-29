#if !defined(ACTUATOR_PROFILE_H)
#define ACTUATOR_PROFILE_H

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Arduino.h>

/**
 * @brief Loads the actuator profile from a JSON file
 * 
 */
class ActuatorProfile {
public:
  struct {
    uint8_t pin;
    uint8_t mode;
  } amberLED;
  struct {
    uint8_t pin;
    uint8_t mode;
  } redLED;
  struct {
    uint8_t pin;
    uint8_t mode;
  } fan;
  /**
   * @brief Constructs a new Actuator Profile object from a JSON file stored in
   * the ESP 32 filesystem
   * @note Compute the member class size using the assistant from 
   * https://arduinojson.org/v6/assistant/
   */
  ActuatorProfile() { }
  /**
   * @brief Destroys the Actuator Profile object
   * 
   */
  ~ActuatorProfile() { }
  /**
   * @brief Parses a JSON file to initialize all variables of structures 
   * amberLED, redLED, and fan
   * @param filename The name of the JSON configuration file
   * @returns true if the JSON file is correctly parsed, otherwise, false
   */
  bool begin(const char*, HardwareSerial&);
private:
  // @brief The size of the JSON doc data structure
  static const uint16_t size = 382;
  StaticJsonDocument<size> doc;
  std::unique_ptr<char[]> buffer;
};

#endif // ACTUATOR_PROFILE_H