#include "DataLogger.h"

bool DataLogger::begin() {
  _ready = false;

  pinMode(SD_CS_PIN, OUTPUT);

  if (!SD.begin(SD_CS_PIN)) {
    return false;
  }

  if (!createFile()) {
    return false;
  }

  writeHeader();
  _ready = true;
  return true;
}

bool DataLogger::ready() const {
  return _ready;
}

const char* DataLogger::filename() const {
  return _filename;
}

bool DataLogger::createFile() {
  for (uint8_t i = 1; i < 100; i++) {
    snprintf(_filename, sizeof(_filename), "LOG%03u.CSV", i);

    if (!SD.exists(_filename)) {
      File file = SD.open(_filename, FILE_WRITE);

      if (!file) {
        return false;
      }

      file.close();
      return true;
    }
  }

  return false;
}

void DataLogger::writeHeader() {
  File file = SD.open(_filename, FILE_WRITE);

  if (!file) {
    _ready = false;
    return;
  }

  file.println(F("Time_ms,MissionState,ControlMode,ServoReady,GPSFix,Satellites,GPSAge_ms,Latitude,Longitude,HomeLat,HomeLon,TargetLat,TargetLon,CMPSReady,Heading_deg,Pitch_deg,Roll_deg,Calibration,WindDirection_deg,WindRaw,AbsoluteWindDirection_deg,WindSpeed,DistanceToTarget_m,BearingToTarget_deg,DesiredHeading_deg,HeadingError_deg,RelativeWindAngle_deg,RudderAngle_deg,SailAngle_deg"));
  file.close();
}

void DataLogger::log(
  const char* missionState,
  const char* controlMode,
  bool servoReady,
  bool gpsFix,
  uint32_t satellites,
  uint32_t gpsAgeMs,
  double latitude,
  double longitude,
  double homeLat,
  double homeLon,
  double targetLat,
  double targetLon,
  bool compassReady,
  float heading,
  int8_t pitch,
  int8_t roll,
  uint8_t calibration,
  float windDirection,
  int windRawDirection,
  float absoluteWindDirection,
  float windSpeed,
  float distanceToTarget,
  float bearingToTarget,
  float desiredHeading,
  float headingError,
  float relativeWindAngle,
  float rudderAngle,
  float sailAngle
) {
  if (!_ready) return;

  File file = SD.open(_filename, FILE_WRITE);

  if (!file) {
    _ready = false;
    return;
  }

  file.print(millis()); file.print(',');
  file.print(missionState); file.print(',');
  file.print(controlMode); file.print(',');
  file.print(servoReady ? 1 : 0); file.print(',');
  file.print(gpsFix ? 1 : 0); file.print(',');
  file.print(satellites); file.print(',');
  file.print(gpsAgeMs); file.print(',');
  file.print(latitude, 6); file.print(',');
  file.print(longitude, 6); file.print(',');
  file.print(homeLat, 6); file.print(',');
  file.print(homeLon, 6); file.print(',');
  file.print(targetLat, 6); file.print(',');
  file.print(targetLon, 6); file.print(',');
  file.print(compassReady ? 1 : 0); file.print(',');
  file.print(heading, 1); file.print(',');
  file.print(pitch); file.print(',');
  file.print(roll); file.print(',');
  file.print(calibration); file.print(',');
  file.print(windDirection, 1); file.print(',');
  file.print(windRawDirection); file.print(',');
  file.print(absoluteWindDirection, 1); file.print(',');
  file.print(windSpeed, 2); file.print(',');
  file.print(distanceToTarget, 1); file.print(',');
  file.print(bearingToTarget, 1); file.print(',');
  file.print(desiredHeading, 1); file.print(',');
  file.print(headingError, 1); file.print(',');
  file.print(relativeWindAngle, 1); file.print(',');
  file.print(rudderAngle, 1); file.print(',');
  file.println(sailAngle, 1);

  file.close();
}
