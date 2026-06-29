#include "RCReceiver.h"

static volatile uint32_t rudderRiseUs = 0;
static volatile uint32_t sailRiseUs = 0;
static volatile uint16_t rudderPulseUs = 0;
static volatile uint16_t sailPulseUs = 0;
static volatile uint32_t rudderLastUs = 0;
static volatile uint32_t sailLastUs = 0;

static float limitRudder(float value) {
  float low = min(RUDDER_LEFT_DEG, RUDDER_RIGHT_DEG);
  float high = max(RUDDER_LEFT_DEG, RUDDER_RIGHT_DEG);
  return clampFloat(value, low, high);
}

static void rudderChange() {
  if (digitalRead(RC_RUDDER_PIN) == HIGH) {
    rudderRiseUs = micros();
  } else {
    uint32_t width = micros() - rudderRiseUs;
    if (width >= RC_SIGNAL_MIN_VALID && width <= RC_SIGNAL_MAX_VALID) {
      rudderPulseUs = width;
      rudderLastUs = micros();
    }
  }
}

static void sailChange() {
  if (digitalRead(RC_SAIL_PIN) == HIGH) {
    sailRiseUs = micros();
  } else {
    uint32_t width = micros() - sailRiseUs;
    if (width >= RC_SIGNAL_MIN_VALID && width <= RC_SIGNAL_MAX_VALID) {
      sailPulseUs = width;
      sailLastUs = micros();
    }
  }
}

void RCReceiver::begin() {
  pinMode(RC_RUDDER_PIN, INPUT);
  pinMode(RC_SAIL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RC_RUDDER_PIN), rudderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RC_SAIL_PIN), sailChange, CHANGE);
}

void RCReceiver::update() {
  uint16_t rudderCopy;
  uint16_t sailCopy;
  uint32_t rudderTimeCopy;
  uint32_t sailTimeCopy;

  noInterrupts();
  rudderCopy = rudderPulseUs;
  sailCopy = sailPulseUs;
  rudderTimeCopy = rudderLastUs;
  sailTimeCopy = sailLastUs;
  interrupts();

  uint32_t nowUs = micros();

  _rudderValid = validPulse(rudderCopy) && ((nowUs - rudderTimeCopy) < RC_SIGNAL_TIMEOUT_US);
  _sailValid = validPulse(sailCopy) && ((nowUs - sailTimeCopy) < RC_SIGNAL_TIMEOUT_US);

  if (_rudderValid) _rudderPulse = rudderCopy;
  if (_sailValid) _sailPulse = sailCopy;
}

bool RCReceiver::connected() const {
  return _rudderValid && _sailValid;
}

float RCReceiver::rudderAngle() const {
  if (!_rudderValid) return RUDDER_CENTRE_DEG;

  uint16_t pulse = constrain(_rudderPulse, RC_RUDDER_MIN_US, RC_RUDDER_MAX_US);

  if (abs((int)pulse - RC_RUDDER_MID_US) <= RC_DEADBAND_US) {
    return RUDDER_CENTRE_DEG;
  }

  float angle;

  if (pulse < RC_RUDDER_MID_US) {
    angle = map(pulse, RC_RUDDER_MIN_US, RC_RUDDER_MID_US, RUDDER_LEFT_DEG, RUDDER_CENTRE_DEG);
  } else {
    angle = map(pulse, RC_RUDDER_MID_US, RC_RUDDER_MAX_US, RUDDER_CENTRE_DEG, RUDDER_RIGHT_DEG);
  }

  if (RC_RUDDER_REVERSE) {
    angle = RUDDER_CENTRE_DEG - (angle - RUDDER_CENTRE_DEG);
  }

  return limitRudder(angle);
}

float RCReceiver::sailAngle() const {
  if (!_sailValid) return SAIL_STOP_DEG;

  uint16_t pulse = constrain(_sailPulse, RC_SAIL_MIN_US, RC_SAIL_MAX_US);

  if (abs((int)pulse - RC_SAIL_MID_US) <= RC_DEADBAND_US) {
    return SAIL_STOP_DEG;
  }

  float output;

  if (pulse < RC_SAIL_MID_US) {
    output = map(pulse, RC_SAIL_MIN_US, RC_SAIL_MID_US, SAIL_OUT_DEG, SAIL_STOP_DEG);
  } else {
    output = map(pulse, RC_SAIL_MID_US, RC_SAIL_MAX_US, SAIL_STOP_DEG, SAIL_IN_DEG);
  }

  if (RC_SAIL_REVERSE) {
    output = SAIL_STOP_DEG - (output - SAIL_STOP_DEG);
  }

  return clampFloat(output, SAIL_MIN_DEG, SAIL_MAX_DEG);
}

uint16_t RCReceiver::rudderPulse() const {
  return _rudderPulse;
}

uint16_t RCReceiver::sailPulse() const {
  return _sailPulse;
}

bool RCReceiver::validPulse(uint16_t pulse) const {
  return pulse >= RC_SIGNAL_MIN_VALID && pulse <= RC_SIGNAL_MAX_VALID;
}
