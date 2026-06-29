#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Serial
#define DEBUG_SERIAL Serial
#define GPS_SERIAL Serial2
#define CMPS_SERIAL Serial3

// GPS pins: TX to Mega RX2 D17, RX to Mega TX2 D16
#define GPS_BAUD 9600

// CMPS12 UART3 pins: CMPS12 TX to Mega RX3 D15, CMPS12 RX to Mega TX3 D14, MODE to GND
#define CMPS_BAUD 9600
#define CMPS_TIMEOUT_MS 250

// PCA9685 servo driver pins: SDA D20, SCL D21
#define SERVO_DRIVER_ADDRESS 0x40
#define SERVO_FREQ 50
#define SAIL_CHANNEL 0
#define RUDDER_CHANNEL 3
#define SERVO_MIN_PULSE 0
#define SERVO_MAX_PULSE 400

// Rudder and sail limits
#define RUDDER_LEFT_DEG 130.0f
#define RUDDER_CENTRE_DEG 85.0f
#define RUDDER_RIGHT_DEG 40.0f
#define SAIL_STOP_DEG 90.0f
#define SAIL_OUT_DEG 75.0f
#define SAIL_IN_DEG 200.0f
#define SAIL_MIN_DEG 0.0f
#define SAIL_MAX_DEG 600.0f
#define SAIL_SAFE_DEG SAIL_STOP_DEG

// Wind sensor pins: wind speed D18, wind direction A14
#define WIND_SPEED_PIN 18
#define WIND_VANE_PIN A14
#define WIND_SAMPLE_MS 1000
#define WIND_SPEED_FACTOR 0.75f
#define WIND_VANE_OFFSET_DEG 0.0f

// Navigation
#define DEFAULT_TARGET_LAT 52.486200
#define DEFAULT_TARGET_LON -1.890400
#define WAYPOINT_REACHED_METERS 3.0f
#define NO_GO_ZONE_DEG 45.0f
#define TACK_ANGLE_DEG 55.0f
#define CONTROL_INTERVAL_MS 250
#define TELEMETRY_INTERVAL_MS 1000

// SD card pins: CS D53, SCK D52, MOSI D51, MISO D50
#define SD_CS_PIN 53
#define LOG_INTERVAL_MS 1000

// RC receiver pins: CH1 rudder D2, CH4 sail D3
#define RC_RUDDER_PIN 2
#define RC_SAIL_PIN 3
#define RC_SIGNAL_MIN_VALID 850
#define RC_SIGNAL_MAX_VALID 2200
#define RC_RUDDER_MIN_US 1000
#define RC_RUDDER_MID_US 1500
#define RC_RUDDER_MAX_US 2000
#define RC_SAIL_MIN_US 1000
#define RC_SAIL_MID_US 1500
#define RC_SAIL_MAX_US 2000
#define RC_DEADBAND_US 40
#define RC_SIGNAL_TIMEOUT_US 120000UL
#define RC_RUDDER_REVERSE false
#define RC_SAIL_REVERSE false

#endif
