#include "MissionControl.h"

void MissionControl::begin() {
  _state = WAITING_FOR_HOME_FIX;
  _homeSaved = false;
  _targetLat = DEFAULT_TARGET_LAT;
  _targetLon = DEFAULT_TARGET_LON;
}

void MissionControl::update(bool gpsFix, double lat, double lon, float distanceToTarget) {
  if (_state == MISSION_COMPLETE) return;

  if (!_homeSaved) {
    if (!gpsFix) return;

    _homeLat = lat;
    _homeLon = lon;
    _homeSaved = true;
    _targetLat = DEFAULT_TARGET_LAT;
    _targetLon = DEFAULT_TARGET_LON;
    _state = SAILING_TO_TARGET;
    return;
  }

  if (!gpsFix) return;

  if (_state == SAILING_TO_TARGET && distanceToTarget <= WAYPOINT_REACHED_METERS) {
    _targetLat = _homeLat;
    _targetLon = _homeLon;
    _state = RETURNING_HOME;
    return;
  }

  if (_state == RETURNING_HOME && distanceToTarget <= WAYPOINT_REACHED_METERS) {
    _state = MISSION_COMPLETE;
  }
}

MissionState MissionControl::state() const {
  return _state;
}

const char* MissionControl::stateName() const {
  switch (_state) {
    case WAITING_FOR_HOME_FIX: return "WAITING_FOR_HOME_FIX";
    case SAILING_TO_TARGET: return "SAILING_TO_TARGET";
    case RETURNING_HOME: return "RETURNING_HOME";
    case MISSION_COMPLETE: return "MISSION_COMPLETE";
    default: return "UNKNOWN";
  }
}

bool MissionControl::homeSaved() const {
  return _homeSaved;
}

double MissionControl::homeLat() const {
  return _homeLat;
}

double MissionControl::homeLon() const {
  return _homeLon;
}

double MissionControl::targetLat() const {
  return _targetLat;
}

double MissionControl::targetLon() const {
  return _targetLon;
}
