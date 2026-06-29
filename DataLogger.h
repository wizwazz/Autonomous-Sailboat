#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Config.h"

class DataLogger {
  public:
    bool begin();
    bool ready() const;
    const char* filename() const;

    void log(
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
    );

  private:
    bool _ready = false;
    char _filename[13];
    bool createFile();
    void writeHeader();
};

#endif
