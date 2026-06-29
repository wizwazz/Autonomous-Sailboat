#ifndef COMPASS_CMPS12_SERIAL_H
#define COMPASS_CMPS12_SERIAL_H

#include <Arduino.h>
#include "Config.h"
#include "Utils.h"

class CompassCMPS12Serial {
  public:
    void begin();
    bool update();
    bool ready() const;
    float heading() const;
    int8_t pitch() const;
    int8_t roll() const;
    uint8_t calibration() const;

  private:
    bool readBytes(uint8_t command, uint8_t *buffer, uint8_t length);

    bool _ready = false;
    float _heading = 0.0f;
    int8_t _pitch = 0;
    int8_t _roll = 0;
    uint8_t _calibration = 0;
    unsigned long _lastCalibrationRead = 0;
};

#endif
