
#include "CO2Sensor.h"

CO2Sensor::CO2Sensor() 
  : i2cDev(nullptr)
  , dataBuffer{}
  , alarmStatus(false)
{

}

void CO2Sensor::begin(TwoWire& i2cDev, const char* modelDev)
{
  this->i2cDev = &i2cDev;
  this->modelDev = modelDev;
  readSerialNumber();
  readSoftwareVersion();
}

void CO2Sensor::readFrame(uint8_t command, uint8_t len, ProcessFrameFn fn, uint8_t* dataFrame=nullptr)
{
  i2cDev->beginTransmission(HW_ADDRESS);
  i2cDev->write(command);
  if (dataFrame) {
    uint8_t dataFrameLength = command == CALIBRATE ? 2 : 6;
    for (uint8_t i = 0; i < dataFrameLength; i++)
      i2cDev->write(dataFrame[i]);
  }
  i2cDev->endTransmission();
  delay(500);
  i2cDev->requestFrom(HW_ADDRESS, int(len));
  if (i2cDev->available() == len)
  {
    for (uint8_t i = 0; i < len; i++)
      dataBuffer[i] = i2cDev->read();
    if (checksum(len)) {
      status = CHECKSUM_OK;
      if (fn)
        (*this.*fn)();
    }
    else
      status = CHECKSUM_ERROR;
  }
  else
    status = INCOMPLETE_FRAME;
}

void CO2Sensor::readSerialNumber()
{
  readFrame(SENSORSN, SNFRAMELENGTH, &CO2Sensor::processSerialNumberFrame);
}

void CO2Sensor::readSoftwareVersion()
{
  readFrame(SENSORSV, SVFRAMELENGTH, &CO2Sensor::processSoftwareVersionFrame);
}

uint16_t CO2Sensor::read()
{
  readFrame(READCO2, DATAFRAMELENGTH, &CO2Sensor::processDataFrame);
  return CO2;
}

bool CO2Sensor::calibrateSensor(uint16_t val, uint8_t pin) {
  bool success = false;
  if (val >= 400 && val <= 1500) {
    uint8_t holdingTime = 0;
    while(!digitalRead(pin)) {
      delay(1000);
      holdingTime++;
    }
    if (holdingTime >= 10) {
      dataBuffer[0] = (val & 0x0700) >> 8;
      dataBuffer[1] = (val & 0x00FF);
      readFrame(CALIBRATE, CALIBRATIONFRAMELENGTH, nullptr, dataBuffer);
      success = true;
    }
  }
  return success;
}

const String CO2Sensor::getSoftwareVersion()
{
  return softwareVersion;
}

const uint8_t* CO2Sensor::getSerialNumber()
{
  return serialNumber;
}

const String CO2Sensor::getStatus()
{
  return status;
}

const String CO2Sensor::getCO2()
{
  String reading(CO2);
  return reading;
}

const String CO2Sensor::getPrintableDataFrame()
{
  dataFrame.clear();
  for (uint8_t i = 0; i < DATAFRAMELENGTH; i++)
    dataFrame =+ dataBuffer[i];
  return dataFrame;
}

const uint8_t* CO2Sensor::getDataBuffer()
{
  return dataBuffer;
}

bool CO2Sensor::checksum(uint8_t len)
{
  uint8_t accum = 0;
  for (uint8_t i = 0; i < len - 1; ++i) 
    accum -= dataBuffer[i];
  uint8_t cs = accum - dataBuffer[len - 1];
  return cs == 0;
}

void CO2Sensor::processSerialNumberFrame()
{
  for (uint8_t i = 0; i < SNLENGTH; i++)
    serialNumber[i] = dataBuffer[i + 1];
  status = STATUS_OK;
}

void CO2Sensor::processSoftwareVersionFrame()
{
  for (uint8_t i = 0; i < SVLENGTH; i++)
    softwareVersion += char(dataBuffer[i + 1]);
  status = STATUS_OK;
}

void CO2Sensor::processDataFrame()
{
  statusByte = dataBuffer[3];
  CO2 = 0;
  switch (statusByte)
  {
  case 0:
    CO2 = (dataBuffer[1] << 8) + dataBuffer[2];
    status = STATUS_OK;
    break;
  case 1:
    status = PREHEATING;
    break;
  case 2:
    status = SENSOR_ERROR;
    break;
  case 4:
    status = OVER_MR;
    break;
  case 8:
    status = LESS_THAN_MR;
  case 16:
    status = NON_CALIBRATED;
    break;
  case 32:
    status = LIGHT_AGING;
    break;
  case 64:
    status = DRIFT;
    break;
  default:
    status = UNKNOWN_ERROR;
  }
}

void CO2Sensor::printSerialNumber(HardwareSerial& serial)
{
  serial.print("Serial number: ");
  for (uint8_t i = 0; i < SNLENGTH; i++)
    serial.printf("%x ", serialNumber[i]);
  serial.println();
}

void CO2Sensor::printDataFrame(HardwareSerial& serial)
{
  serial.print("Data frame: ");
  for (uint8_t i = 0; i < DATAFRAMELENGTH; i++)
    serial.printf("%x ", dataBuffer[i]);
  serial.println();
}

