#include "Utils.h"

float wrap360(float angle) {
  while (angle >= 360.0f) angle -= 360.0f;
  while (angle < 0.0f) angle += 360.0f;
  return angle;
}

float angleDifference(float target, float current) {
  float diff = target - current;
  while (diff > 180.0f) diff -= 360.0f;
  while (diff < -180.0f) diff += 360.0f;
  return diff;
}

float clampFloat(float value, float low, float high) {
  if (value < low) return low;
  if (value > high) return high;
  return value;
}
