#ifndef WIND_SENSOR_H
#define WIND_SENSOR_H

#include <Arduino.h>
#include "Config.h"
#include "Utils.h"

class WindSensor {
  public:
    void begin();
    void update();
    float speed() const;
    float direction() const;
    int rawDirection() const;

  private:
    float _speed = 0.0f;
    float _direction = 0.0f;
    int _rawDirection = 0;
    unsigned long _lastSample = 0;
    unsigned long _lastPulses = 0;
};

#endif
