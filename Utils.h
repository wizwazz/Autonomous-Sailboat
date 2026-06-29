#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

float wrap360(float angle);
float angleDifference(float target, float current);
float clampFloat(float value, float low, float high);

#endif
