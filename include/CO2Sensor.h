#if !defined(CO2_SENSOR_H)
#define CO2_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

#define CM1107_MODEL 1107 //!< Key that identifies the CM1107 module
#define CM1106_MODEL 1106 //!< Key that identifies the CM1106 module

#define HW_ADDRESS 0x31 //!< Hardware address

#define READCO2    0x01 //!< Command to query a reading from the CO2 sensor
#define SETABC     0x10 //!< Command to set ABC parameter
#define CALIBRATE  0x03 //!< Command to calibrate value of CO2
#define SENSORSV   0x1E //!< Command to read the software version of the sensor
#define SENSORSN   0x1F //!< Command to read the serial number of the sensor

#define SVLENGTH   0x0A //!< The length of the software version
#define SNLENGTH   0x0A //!< The length of the serial number

#define SVFRAMELENGTH   0x0C //!< The length of the software version frame
#define SNFRAMELENGTH   0x0C //!< The length of the serial number frame
#define DATAFRAMELENGTH 0x05 //!< The length of the serial number frame

#define STATUS_OK        "OK"              
#define INCOMPLETE_FRAME "Incomplete frame"
#define CHECKSUM_ERROR   "Checksum fails"
#define DRIFT            "Drift"
#define LIGHT_AGING      "Light aging"
#define NON_CALIBRATED   "Non-calibrated"
#define LESS_THAN_MR     "Less than measurement range"
#define OVER_MR          "Over measurement range"
#define SENSOR_ERROR     "Sensor error"
#define PREHEATING       "Preheating"
#define UNKNOWN_ERROR    "Unknown error"

/**
 * Main class intended for read data from a CUBIC's CM110x Dual Beam NDIR CO2 
 * Sensor 
 * I2C is the protocol that objects from this class uses to communicate 
 * with the sensor module.
 * I2C clock frequency must be set in the range [10000, 400000] Hz to get
 * readings correctly. Low clock frequencies are prefered.
 * This class supports both the CM1106 and CM1107 sensor modules.
 * CM1106 is a highly power-efficient CO2 sensor module.
 * CM1107 is a highlt-accurate CO2 sensor module.
 */
class CO2Sensor
{
private:
    /// A pointer to an I2C instance created by the user
    TwoWire* i2cDev;
    /// Keeps received frames from the sensor module
    uint8_t dataBuffer[12];
    /// The last status the MCU receives from the sensor when reading CO2
    uint8_t statusByte;
    /// The model of the sensor module (CM1106 or CM1107)
    uint16_t modelDev;
    /// The serial number of the sensor module
    uint8_t serialNumber[10];
    /// The software version of the sensor module
    String softwareVersion;
    /// The status of the last communication with the sensor module
    String status;
    /// The status of the alarm
    bool alarmStatus;
    /// CO2 measurement
    uint16_t CO2;
private:
    typedef void (CO2Sensor::*ProcessFrameFn)(void);
    /**
     * The procedure subtracts values of the dataBuffer array in the range 
     * [0, last) from a zero-initialized accumulator. Then, it subtracts the 
     * last element from the accumulator. If the difference equals zero, then 
     * the checksum is OK, otherwise, the array is corrupted. This procedure 
     * discards the carry.
     * 
     * @brief Computes the checksum of the dataBuffer array. 
     * @param len The length of the data frame.
     * @returns True when the checksum is OK, otherwise False.
     */
    bool checksum(uint8_t);
    /**
     * @brief Reads the serial number of the sensor module.
     * 
     * This member function reads the serial number of the sensor, then it 
     * holds the value in the serialNumber buffer.
     */
    void readSerialNumber();
    /**
     * @brief Reads the software version of the sensor module.
     * 
     * This member function reads the software version of the sensor module,
     * then it holds the value in the softwareVersion buffer.
     * 
     */
    void readSoftwareVersion();
    /**
     * @brief Reads a frame from the sensor module
     * 
     * This member function reads a frame an processes it according a 
     * member function.
     * @param command A valid sensor module command
     * @param len The length of the frame
     * @param mf A nostatic CO2Sensor member function that process the frame
     */
    void readFrame(uint8_t, uint8_t, ProcessFrameFn);
    /**
     * @brief Process the serial number frame. This procedure fills the serial 
     * number buffer from the data buffer. Also, this sets the status
     * according to the result of the frame processing.
     */
    void processSerialNumberFrame();
    /**
     * @brief Process the software version frame. This procedure fills the 
     * software version string from the data buffer. Also, this sets the status
     * according to the result of the frame processing.
     */
    void processSoftwareVersionFrame();
    /**
     * @brief Process a data frame. This procedure assigns to the variable CO2 
     * a value processed from the data buffer. Also, this sets the status
     * according to the result of the frame processing.
     */
    void processDataFrame();
public:
    /**
     * @brief Constructs a new CO2Sensor object.
     * 
     */
    CO2Sensor();
    /**
     * @brief Sets the I2C class attribute.
     * @param i2c A reference to an TwoWire object instanced by the user.
     * @param model The model of the CO2 Sensor module (default CM1107).
     */
    void begin(TwoWire&, uint16_t);
    /**
     * @brief Reads a CO2 measurement.
     * @returns A 16-bit value that contains the CO2 concentration (PPM).
     */
    uint16_t read();
    /**
     * @brief Gets the sensor module's serial number.
     * @returns A pointer to the 10-byte buffer that holds the serial number.
     * @note the HEX format is prefered to print this value.
     */
    const uint8_t* getSerialNumber();
    /**
     * @brief Gets the sensor module's software version.
     * @returns An Arduino-C string holding the software version.
     */
    const String getSoftwareVersion();
    /**
     * @brief Gets the current status of the sensor module.
     * @returns An Arduino-C string holding the current status.
     */
    const String getStatus();
    /**
     * @brief Gets the last frame the MCU receives.
     * @returns A pointer to an array holding the last received 5-byte data 
     *     frame.
     */
    const uint8_t* getDataFrame();
    /**
     * @brief Prints the sensor module's serial number
     * @param hs a hardware serial object
     */
    void printSerialNumber(HardwareSerial&);
    /**
     * @brief Prints the sensor module's software version
     * @param hs a hardware serial object
     */
    inline void printSoftwareVersion(HardwareSerial&);
    /**
     * @brief Prints the last received data frame
     * @param hs a hardware serial object
     */
    void printDataFrame(HardwareSerial&);
    /**
     * @brief Prints the last received data frame
     * @param hs a hardware serial object
     */
    inline void printCO2Measurement(HardwareSerial&);
    /**
     * @brief Prints the last status
     * @param hs a hardware serial object
     */
    inline void printStatus(HardwareSerial&);
};

inline void CO2Sensor::printCO2Measurement(HardwareSerial& serial)
{
  serial.printf("CO2 measurement: %i\n", CO2);
}

inline void CO2Sensor::printSoftwareVersion(HardwareSerial& serial)
{
  serial.printf("Software version: %s\n", softwareVersion.c_str());
}

inline void CO2Sensor::printStatus(HardwareSerial& serial)
{
  serial.printf("Status: %s\n", status.c_str());
}

#endif // CO2_SENSOR_H


