#ifndef GPS_MODULE_H
#define GPS_MODULE_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "Config.h"

class GPSModule {
  public:
    void begin();
    void update();
    bool fixValid() const;
    double latitude() const;
    double longitude() const;
    uint32_t satellites() const;
    uint32_t ageMs() const;

  private:
    TinyGPSPlus gps;
};

#endif
