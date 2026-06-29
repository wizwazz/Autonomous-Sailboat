#include "ServoController.h"

ServoController::ServoController()
  : pwm(SERVO_DRIVER_ADDRESS) {
}

bool ServoController::begin() {
  _ready = false;

  if (!deviceFound()) {
    return false;
  }

  pwm.begin();
  delay(50);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(50);

  _ready = true;
  return true;
}

bool ServoController::ready() const {
  return _ready;
}

bool ServoController::deviceFound() {
  Wire.beginTransmission(SERVO_DRIVER_ADDRESS);
  return Wire.endTransmission() == 0;
}

void ServoController::setSailAngle(float angle) {
  if (!_ready) return;
  setServoAngle(SAIL_CHANNEL, angle);
}

void ServoController::setRudderAngle(float angle) {
  if (!_ready) return;
  setServoAngle(RUDDER_CHANNEL, angle);
}

void ServoController::neutral() {
  if (!_ready) return;
  setSailAngle(SAIL_SAFE_DEG);
  setRudderAngle(RUDDER_CENTRE_DEG);
}

void ServoController::setServoAngle(uint8_t channel, float angle) {
  angle = clampFloat(angle, 0.0f, 180.0f);
  int pulse = map((int)angle, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
  pwm.setPWM(channel, 0, pulse);
}
