#include "util.h"

float degrees_to_radians(float degrees) { return degrees * M_PI / 180.0; }

float random_float(void) { return rand() / (RAND_MAX + 1.0); }

float random_float_range(float min, float max) {
  return min + (max - min) * random_float();
}

float clamp(float v, float min, float max) {
  return (v < min) ? min : (v > max) ? max : v;
}
