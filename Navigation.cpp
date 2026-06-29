#include "Navigation.h"

float Navigation::distanceMeters(double lat1, double lon1, double lat2, double lon2) {
  const double radius = 6371000.0;
  double phi1 = radians(lat1);
  double phi2 = radians(lat2);
  double dPhi = radians(lat2 - lat1);
  double dLambda = radians(lon2 - lon1);

  double a = sin(dPhi / 2.0) * sin(dPhi / 2.0) +
             cos(phi1) * cos(phi2) *
             sin(dLambda / 2.0) * sin(dLambda / 2.0);

  double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
  return radius * c;
}

float Navigation::bearingDegrees(double lat1, double lon1, double lat2, double lon2) {
  double phi1 = radians(lat1);
  double phi2 = radians(lat2);
  double lambda1 = radians(lon1);
  double lambda2 = radians(lon2);
  double dLambda = lambda2 - lambda1;

  double y = sin(dLambda) * cos(phi2);
  double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(dLambda);

  return wrap360(degrees(atan2(y, x)));
}

float Navigation::desiredHeading(float targetBearing, float windDirection) {
  float windToTarget = abs(angleDifference(targetBearing, windDirection));

  if (windToTarget < NO_GO_ZONE_DEG) {
    float leftTack = wrap360(windDirection - TACK_ANGLE_DEG);
    float rightTack = wrap360(windDirection + TACK_ANGLE_DEG);

    float leftError = abs(angleDifference(targetBearing, leftTack));
    float rightError = abs(angleDifference(targetBearing, rightTack));

    if (leftError < rightError) return leftTack;
    return rightTack;
  }

  return wrap360(targetBearing);
}

float Navigation::rudderAngle(float headingError) {
  float rudder;

  if (headingError < 0.0f) {
    rudder = map(
      constrain((int)headingError, -90, 0),
      -90,
      0,
      RUDDER_LEFT_DEG,
      RUDDER_CENTRE_DEG
    );
  } else {
    rudder = map(
      constrain((int)headingError, 0, 90),
      0,
      90,
      RUDDER_CENTRE_DEG,
      RUDDER_RIGHT_DEG
    );
  }

  float low = min(RUDDER_LEFT_DEG, RUDDER_RIGHT_DEG);
  float high = max(RUDDER_LEFT_DEG, RUDDER_RIGHT_DEG);
  return clampFloat(rudder, low, high);
}

float Navigation::sailAngle(float relativeWindAngle) {
  return SAIL_STOP_DEG;
}
