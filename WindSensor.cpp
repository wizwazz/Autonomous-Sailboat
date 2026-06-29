#include "WindSensor.h"

static volatile unsigned long windPulseCount = 0;

static void windPulseISR() {
  windPulseCount++;
}

void WindSensor::begin() {
  pinMode(WIND_SPEED_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIND_SPEED_PIN), windPulseISR, FALLING);
  _lastSample = millis();
}

void WindSensor::update() {
  _rawDirection = analogRead(WIND_VANE_PIN);
  _direction = wrap360((_rawDirection * 359.0f / 1023.0f) + WIND_VANE_OFFSET_DEG);

  unsigned long now = millis();

  if (now - _lastSample >= WIND_SAMPLE_MS) {
    noInterrupts();
    unsigned long currentPulses = windPulseCount;
    interrupts();

    unsigned long pulseDifference = currentPulses - _lastPulses;
    float seconds = (now - _lastSample) / 1000.0f;

    _speed = (pulseDifference / seconds) * WIND_SPEED_FACTOR;
    _lastPulses = currentPulses;
    _lastSample = now;
  }
}

float WindSensor::speed() const {
  return _speed;
}

float WindSensor::direction() const {
  return _direction;
}

int WindSensor::rawDirection() const {
  return _rawDirection;
}
