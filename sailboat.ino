#include <Wire.h>
#include "Config.h"
#include "GPSModule.h"
#include "CompassCMPS12Serial.h"
#include "WindSensor.h"
#include "ServoController.h"
#include "Navigation.h"
#include "MissionControl.h"
#include "DataLogger.h"
#include "RCReceiver.h"
#include "Utils.h"

GPSModule gps;
CompassCMPS12Serial compass;
WindSensor wind;
ServoController servos;
MissionControl mission;
DataLogger logger;
RCReceiver rc;
ControlData control;

bool servoReady = false;
bool sdReady = false;
bool manualMode = false;
unsigned long lastControlMs = 0;
unsigned long lastTelemetryMs = 0;
unsigned long lastLogMs = 0;

void setup() {
  // Serial
  DEBUG_SERIAL.begin(115200);
  delay(1500);
  DEBUG_SERIAL.println(F("BOOT 1: Serial started"));

  // PCA9685 I2C pins: SDA D20, SCL D21
  Wire.begin();
  #if defined(WIRE_HAS_TIMEOUT)
    Wire.setWireTimeout(3000, true);
  #endif
  DEBUG_SERIAL.println(F("BOOT 2: I2C started"));

  // GPS pins: TX to RX2 D17, RX to TX2 D16
  gps.begin();
  DEBUG_SERIAL.println(F("BOOT 3: GPS started"));

  // CMPS12 UART3 pins: TX to RX3 D15, RX to TX3 D14, MODE to GND
  compass.begin();
  DEBUG_SERIAL.println(F("BOOT 4: CMPS12 UART3 started"));

  // Wind pins: speed D18, direction A14
  wind.begin();
  DEBUG_SERIAL.println(F("BOOT 5: Wind started"));

  // RC receiver pins: rudder D2, sail D3
  rc.begin();
  DEBUG_SERIAL.println(F("BOOT 6: RC receiver started"));

  // Servo driver pins: SDA D20, SCL D21
  DEBUG_SERIAL.println(F("BOOT 7: Starting servo driver"));
  servoReady = servos.begin();

  if (servoReady) {
    DEBUG_SERIAL.println(F("BOOT 8: Servo driver ready"));
    servos.neutral();
  } else {
    DEBUG_SERIAL.println(F("BOOT 8: Servo driver not found"));
  }

  // SD card pins: CS D53, SCK D52, MOSI D51, MISO D50
  DEBUG_SERIAL.println(F("BOOT 9: Starting SD card"));
  sdReady = logger.begin();

  if (sdReady) {
    DEBUG_SERIAL.print(F("BOOT 10: SD ready "));
    DEBUG_SERIAL.println(logger.filename());
  } else {
    DEBUG_SERIAL.println(F("BOOT 10: SD not ready"));
  }

  mission.begin();
  resetControlData();
  DEBUG_SERIAL.println(F("BOOT 11: System ready"));
}

void loop() {
  gps.update();
  compass.update();
  wind.update();
  rc.update();

  unsigned long now = millis();

  if (now - lastControlMs >= CONTROL_INTERVAL_MS) {
    lastControlMs = now;
    updateControl();
  }

  if (now - lastTelemetryMs >= TELEMETRY_INTERVAL_MS) {
    lastTelemetryMs = now;
    printTelemetry();
  }

  if (now - lastLogMs >= LOG_INTERVAL_MS) {
    lastLogMs = now;
    logData();
  }
}

void resetControlData() {
  control.homeLat = 0.0;
  control.homeLon = 0.0;
  control.targetLat = DEFAULT_TARGET_LAT;
  control.targetLon = DEFAULT_TARGET_LON;
  control.distanceToTargetM = 0.0f;
  control.bearingToTargetDeg = 0.0f;
  control.desiredHeadingDeg = 0.0f;
  control.headingErrorDeg = 0.0f;
  control.relativeWindAngleDeg = 0.0f;
  control.rudderAngleDeg = RUDDER_CENTRE_DEG;
  control.sailAngleDeg = SAIL_SAFE_DEG;
}

void updateControl() {
  bool gpsFix = gps.fixValid();
  bool compassReady = compass.ready();
  manualMode = rc.connected();

  if (servoReady && manualMode) {
    control.rudderAngleDeg = rc.rudderAngle();
    control.sailAngleDeg = rc.sailAngle();
    servos.setRudderAngle(control.rudderAngleDeg);
    servos.setSailAngle(control.sailAngleDeg);
    return;
  }

  if (mission.homeSaved()) {
    control.homeLat = mission.homeLat();
    control.homeLon = mission.homeLon();
  }

  control.targetLat = mission.targetLat();
  control.targetLon = mission.targetLon();

  if (gpsFix) {
    control.distanceToTargetM = Navigation::distanceMeters(
      gps.latitude(), gps.longitude(),
      control.targetLat, control.targetLon
    );

    control.bearingToTargetDeg = Navigation::bearingDegrees(
      gps.latitude(), gps.longitude(),
      control.targetLat, control.targetLon
    );
  }

  mission.update(gpsFix, gps.latitude(), gps.longitude(), control.distanceToTargetM);

  control.targetLat = mission.targetLat();
  control.targetLon = mission.targetLon();

  if (!servoReady) return;

  if (!gpsFix || !compassReady || mission.state() == WAITING_FOR_HOME_FIX || mission.state() == MISSION_COMPLETE) {
    servos.neutral();
    control.rudderAngleDeg = RUDDER_CENTRE_DEG;
    control.sailAngleDeg = SAIL_SAFE_DEG;
    return;
  }

  float relativeWindDirection = wind.direction();
  float absoluteWindDirection = wrap360(compass.heading() + relativeWindDirection);

  control.desiredHeadingDeg = Navigation::desiredHeading(
    control.bearingToTargetDeg,
    absoluteWindDirection
  );

  control.headingErrorDeg = angleDifference(control.desiredHeadingDeg, compass.heading());
  control.relativeWindAngleDeg = angleDifference(absoluteWindDirection, compass.heading());
  control.rudderAngleDeg = Navigation::rudderAngle(control.headingErrorDeg);
  control.sailAngleDeg = Navigation::sailAngle(control.relativeWindAngleDeg);

  servos.setRudderAngle(control.rudderAngleDeg);
  servos.setSailAngle(control.sailAngleDeg);
}

void logData() {
  if (!sdReady) return;

  logger.log(
    mission.stateName(),
    manualMode ? "MANUAL" : "AUTO",
    servoReady,
    gps.fixValid(),
    gps.satellites(),
    gps.ageMs(),
    gps.latitude(),
    gps.longitude(),
    control.homeLat,
    control.homeLon,
    control.targetLat,
    control.targetLon,
    compass.ready(),
    compass.heading(),
    compass.pitch(),
    compass.roll(),
    compass.calibration(),
    wind.direction(),
    wind.rawDirection(),
    compass.ready() ? wrap360(compass.heading() + wind.direction()) : wind.direction(),
    wind.speed(),
    control.distanceToTargetM,
    control.bearingToTargetDeg,
    control.desiredHeadingDeg,
    control.headingErrorDeg,
    control.relativeWindAngleDeg,
    control.rudderAngleDeg,
    control.sailAngleDeg
  );

  if (!logger.ready()) {
    sdReady = false;
    DEBUG_SERIAL.println(F("SD logging stopped"));
  }
}

void printTelemetry() {
  DEBUG_SERIAL.print(F("State="));
  DEBUG_SERIAL.print(mission.stateName());

  DEBUG_SERIAL.print(F(" Mode="));
  DEBUG_SERIAL.print(manualMode ? F("MANUAL") : F("AUTO"));

  DEBUG_SERIAL.print(F(" Servo="));
  DEBUG_SERIAL.print(servoReady ? F("OK") : F("NO"));

  DEBUG_SERIAL.print(F(" SD="));
  DEBUG_SERIAL.print(sdReady ? F("OK") : F("NO"));

  DEBUG_SERIAL.print(F(" GPS="));
  DEBUG_SERIAL.print(gps.fixValid() ? F("OK") : F("NO"));

  DEBUG_SERIAL.print(F(" Sat="));
  DEBUG_SERIAL.print(gps.satellites());

  DEBUG_SERIAL.print(F(" Lat="));
  DEBUG_SERIAL.print(gps.latitude(), 6);

  DEBUG_SERIAL.print(F(" Lon="));
  DEBUG_SERIAL.print(gps.longitude(), 6);

  DEBUG_SERIAL.print(F(" CMPS="));
  DEBUG_SERIAL.print(compass.ready() ? F("OK") : F("NO"));

  DEBUG_SERIAL.print(F(" Head="));
  DEBUG_SERIAL.print(compass.heading(), 1);

  DEBUG_SERIAL.print(F(" Pitch="));
  DEBUG_SERIAL.print(compass.pitch());

  DEBUG_SERIAL.print(F(" Roll="));
  DEBUG_SERIAL.print(compass.roll());

  DEBUG_SERIAL.print(F(" Cal="));
  DEBUG_SERIAL.print(compass.calibration());

  DEBUG_SERIAL.print(F(" WindRaw="));
  DEBUG_SERIAL.print(wind.rawDirection());

  DEBUG_SERIAL.print(F(" WindRel="));
  DEBUG_SERIAL.print(wind.direction(), 1);

  DEBUG_SERIAL.print(F(" WindAbs="));
  DEBUG_SERIAL.print(compass.ready() ? wrap360(compass.heading() + wind.direction()) : wind.direction(), 1);

  DEBUG_SERIAL.print(F(" WindSpd="));
  DEBUG_SERIAL.print(wind.speed(), 2);

  DEBUG_SERIAL.print(F(" Dist="));
  DEBUG_SERIAL.print(control.distanceToTargetM, 1);

  DEBUG_SERIAL.print(F(" Bearing="));
  DEBUG_SERIAL.print(control.bearingToTargetDeg, 1);

  DEBUG_SERIAL.print(F(" Desired="));
  DEBUG_SERIAL.print(control.desiredHeadingDeg, 1);

  DEBUG_SERIAL.print(F(" Error="));
  DEBUG_SERIAL.print(control.headingErrorDeg, 1);

  DEBUG_SERIAL.print(F(" RCPulse="));
  DEBUG_SERIAL.print(rc.rudderPulse());
  DEBUG_SERIAL.print(F("/"));
  DEBUG_SERIAL.print(rc.sailPulse());

  DEBUG_SERIAL.print(F(" Rudder="));
  DEBUG_SERIAL.print(control.rudderAngleDeg, 1);

  DEBUG_SERIAL.print(F(" Sail="));
  DEBUG_SERIAL.println(control.sailAngleDeg, 1);
}
