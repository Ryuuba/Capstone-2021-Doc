#if !defined(WIFI_SETUP_H)
#define WIFI_SETUP_H

#include <Arduino.h>
#include <WiFi.h>

#define MAX_ATTEMPT 15
/**
 * @brief Provides several simplified methods to connect the MCU to a WiFi
 *     work.
 * @author Adán G. Medrano-Chávez ryuuba@gmail.com
 * @todo Add security login. Add context object (FSM). Adjust the default value
 *     of the delay param in constructor.
 */
class WiFiSetup
{
private:
  /// The service set identifier
  const char* ssid;
  /// The password 
  const char* passwd;
  /// Time to wait until the next connection attempt
  uint32_t setupDelay;
public:
  /**
   * @brief Construct a new WiFiSetup object.
   * @param ssid The service set identifier aka WiFi name.
   * @param passwd The password to get WiFi access.
   * @param delay Time to wait until the next connection attempt
   *     (default 500 ms).
   */
  WiFiSetup(const char*, const char*, uint32_t delay = 500);
  /**
   * @brief default destructor
   * 
   */
  ~WiFiSetup() {}
  /**
   * @brief Start the connection with the WiFi Network
   * @param serial A HardwareSerial object to print the connection status
   * @returns True if the connection is established before MAX_ATTEMPTs,
   *     otherwise, returns false (the connection cannot be established)
   */
  bool begin(HardwareSerial&);
};

#endif // WIFI_SETUP_H
