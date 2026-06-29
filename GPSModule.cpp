#include "GPSModule.h"

void GPSModule::begin() {
  GPS_SERIAL.begin(GPS_BAUD);
}

void GPSModule::update() {
  while (GPS_SERIAL.available() > 0) {
    gps.encode(GPS_SERIAL.read());
  }
}

bool GPSModule::fixValid() const {
  return gps.location.isValid() && gps.location.age() < 3000;
}

double GPSModule::latitude() const {
  return gps.location.lat();
}

double GPSModule::longitude() const {
  return gps.location.lng();
}

uint32_t GPSModule::satellites() const {
  if (!gps.satellites.isValid()) return 0;
  return gps.satellites.value();
}

uint32_t GPSModule::ageMs() const {
  if (!gps.location.isValid()) return 999999;
  return gps.location.age();
}
