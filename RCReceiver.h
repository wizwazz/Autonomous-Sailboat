#ifndef RC_RECEIVER_H
#define RC_RECEIVER_H

#include <Arduino.h>
#include "Config.h"
#include "Utils.h"

class RCReceiver {
  public:
    void begin();
    void update();
    bool connected() const;
    float rudderAngle() const;
    float sailAngle() const;
    uint16_t rudderPulse() const;
    uint16_t sailPulse() const;

  private:
    uint16_t _rudderPulse = 0;
    uint16_t _sailPulse = 0;
    bool _rudderValid = false;
    bool _sailValid = false;

    bool validPulse(uint16_t pulse) const;
};

#endif
