#include "WiFiSetup.h"

WiFiSetup::WiFiSetup(const char* ssid, const char* passwd, uint32_t delay)
  : ssid(ssid)
  , passwd(passwd)
  , setupDelay(delay)
{

}

bool WiFiSetup::begin(HardwareSerial& serial)
{
  bool result = false;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  serial.printf("Trying connection to %s\n", ssid);
  for (uint8_t i = 0; i < MAX_ATTEMPT; i++)
  {
    delay(setupDelay);
    if (WiFi.status() != WL_CONNECTED)
      serial.printf("Attempt number: %i\n", i);
    else
    {
      serial.printf("Connection established to SSID %s\n", ssid);
      result = true;
      break;
    }
  }
  return result;
}