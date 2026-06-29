#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include "Config.h"
#include "Utils.h"

struct ControlData {
  double homeLat;
  double homeLon;
  double targetLat;
  double targetLon;
  float distanceToTargetM;
  float bearingToTargetDeg;
  float desiredHeadingDeg;
  float headingErrorDeg;
  float relativeWindAngleDeg;
  float rudderAngleDeg;
  float sailAngleDeg;
};

class Navigation {
  public:
    static float distanceMeters(double lat1, double lon1, double lat2, double lon2);
    static float bearingDegrees(double lat1, double lon1, double lat2, double lon2);
    static float desiredHeading(float targetBearing, float windDirection);
    static float rudderAngle(float headingError);
    static float sailAngle(float relativeWindAngle);
};

#endif
