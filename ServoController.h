#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Config.h"
#include "Utils.h"

class ServoController {
  public:
    ServoController();
    bool begin();
    bool ready() const;
    void setSailAngle(float angle);
    void setRudderAngle(float angle);
    void neutral();

  private:
    Adafruit_PWMServoDriver pwm;
    bool _ready = false;
    bool deviceFound();
    void setServoAngle(uint8_t channel, float angle);
};

#endif
