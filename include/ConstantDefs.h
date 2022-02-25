#if !defined(CONSTANT_DEFS_H)
#define CONSTANT_DEFS_H

// This led indicates whether the mcu is working or not (debug)
#define TEST_LED 25
// The button entering the manual calibration mode
#define CALIBRATION_BUTTON 19
// The value to calibrate the sensor, use a calibrated sensor to set this value
// or use 400 PPM in and fresh-air indoor environment
#define CALIBRATION_TARGET_VALUE 440
// I2C definitions
#define SDA 21
#define SCL 22
#define BASE_FREQ 100000
// OLED definitions
#define OLED_WIDTH 128 // OLED display width, in pixels
#define OLED_HEIGHT 64 // OLED display height, in pixels
#define OLED_ADDRESS 0x3C
#define OLED_FREQ 100000
// MQTT definitions
#define MQTT_BROKER "10.0.0.6"
#define MQTT_PORT 1883
#define CLIENT_ID "ESP32"
#define CO2_TOPIC "esp32/spCO2"
#define TEMPERATURE_TOPIC "esp32/temp"
#define HUMIDITY_TOPIC "esp32/humd"
// WiFi settings
#define SSID_HOME "Ryuuba"
#define PASSWD_HOME "xoloitzcuintle"
#endif // CONSTANT_DEFS_H
