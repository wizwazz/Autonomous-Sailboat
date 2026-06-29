# Autonomous Sailboat

## Overview

This project implements an autonomous RC sailboat using an Arduino Mega 2560. The system combines GPS navigation, compass heading, wind sensing and servo control to autonomously sail towards a predefined waypoint before automatically returning to its launch location. Manual control can also be enabled using an RC transmitter, allowing the operator to override the autonomous system at any time.

## Features

* Autonomous waypoint navigation
* Automatic return-to-home functionality
* Manual RC override
* GPS positioning
* CMPS12 compass and IMU integration
* Wind speed and wind direction sensing
* Rudder and sail servo control
* SD card data logging
* Modular Arduino project structure

## Hardware

* Arduino Mega 2560
* Adafruit PCA9685 16-Channel Servo Driver
* CMPS12 Electronic Compass (UART3)
* GPS Module (UART2)
* Wind Speed Sensor
* Wind Direction Sensor
* SD Card Module
* Rudder Servo
* Sail Winch Servo
* RC Receiver (Manual Override)

## Pin Configuration

| Device           | Arduino Pin |
| ---------------- | ----------- |
| GPS RX           | D17         |
| GPS TX           | D16         |
| CMPS12 RX        | D15         |
| CMPS12 TX        | D14         |
| Servo Driver SDA | D20         |
| Servo Driver SCL | D21         |
| RC Rudder (CH1)  | D2          |
| RC Sail (CH4)    | D3          |
| Wind Speed       | D18         |
| Wind Direction   | A14         |
| SD Card CS       | D53         |

## Project Structure

```text
sailboat.ino
Config.h
Navigation.cpp / .h
GPS.cpp / .h
CompassCMPS12Serial.cpp / .h
WindSensor.cpp / .h
ServoController.cpp / .h
RCReceiver.cpp / .h
Logger.cpp / .h
Utils.cpp / .h
```

## Data Logging

The system records operational data to an SD card in CSV format, including:

* Time
* Latitude
* Longitude
* Heading
* Wind Speed
* Wind Direction
* Rudder Position
* Sail Position
* Navigation Mode
* Target Waypoint

The recorded data can be imported into MATLAB for route visualisation and performance analysis.

## Autonomous Operation

1. Initialise all sensors and hardware.
2. Acquire GPS position and compass heading.
3. Measure wind speed and wind direction.
4. Calculate the bearing and distance to the target waypoint.
5. Compute heading error.
6. Adjust the rudder and sail accordingly.
7. Upon reaching the waypoint, automatically navigate back to the launch position.
8. Continue logging sensor data throughout the mission.

## Manual Override

When a valid RC receiver signal is detected, autonomous control is suspended and manual control is enabled. Once the transmitter is switched off or the signal is lost, the system automatically resumes autonomous navigation.

## Future Improvements

* Adaptive sail trimming
* Obstacle detection and avoidance
* Route optimisation
* Dynamic waypoint generation
* Enhanced sensor filtering
* Wind-aware path planning

## Author

Developed as part of an autonomous sailboat project using Arduino Mega 2560 for autonomous marine navigation.
