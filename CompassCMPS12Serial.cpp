#include "CompassCMPS12Serial.h"

void CompassCMPS12Serial::begin() {
  CMPS_SERIAL.begin(CMPS_BAUD);
}

bool CompassCMPS12Serial::update() {
  uint8_t data[4];

  if (!readBytes(0x23, data, 4)) {
    _ready = false;
    return false;
  }

  uint16_t rawHeading = ((uint16_t)data[0] << 8) | data[1];
  _heading = wrap360(rawHeading / 10.0f);
  _pitch = (int8_t)data[2];
  _roll = (int8_t)data[3];
  _ready = true;

  if (millis() - _lastCalibrationRead >= 1000) {
    uint8_t calData[1];
    if (readBytes(0x24, calData, 1)) {
      _calibration = calData[0];
    }
    _lastCalibrationRead = millis();
  }

  return true;
}

bool CompassCMPS12Serial::ready() const {
  return _ready;
}

float CompassCMPS12Serial::heading() const {
  return _heading;
}

int8_t CompassCMPS12Serial::pitch() const {
  return _pitch;
}

int8_t CompassCMPS12Serial::roll() const {
  return _roll;
}

uint8_t CompassCMPS12Serial::calibration() const {
  return _calibration;
}

bool CompassCMPS12Serial::readBytes(uint8_t command, uint8_t *buffer, uint8_t length) {
  while (CMPS_SERIAL.available() > 0) {
    CMPS_SERIAL.read();
  }

  CMPS_SERIAL.write(command);
  CMPS_SERIAL.flush();

  unsigned long start = millis();
  uint8_t index = 0;

  while (index < length && millis() - start < CMPS_TIMEOUT_MS) {
    if (CMPS_SERIAL.available() > 0) {
      buffer[index++] = CMPS_SERIAL.read();
    }
  }

  return index == length;
}
