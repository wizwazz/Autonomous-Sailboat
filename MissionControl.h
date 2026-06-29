#ifndef MISSION_CONTROL_H
#define MISSION_CONTROL_H

#include <Arduino.h>
#include "Config.h"

enum MissionState {
  WAITING_FOR_HOME_FIX,
  SAILING_TO_TARGET,
  RETURNING_HOME,
  MISSION_COMPLETE
};

class MissionControl {
  public:
    void begin();
    void update(bool gpsFix, double lat, double lon, float distanceToTarget);
    MissionState state() const;
    const char* stateName() const;
    bool homeSaved() const;
    double homeLat() const;
    double homeLon() const;
    double targetLat() const;
    double targetLon() const;

  private:
    MissionState _state = WAITING_FOR_HOME_FIX;
    bool _homeSaved = false;
    double _homeLat = 0.0;
    double _homeLon = 0.0;
    double _targetLat = DEFAULT_TARGET_LAT;
    double _targetLon = DEFAULT_TARGET_LON;
};

#endif
