#include "util.h"

float degrees_to_radians(float degrees) { return degrees * M_PI / 180.0; }

uint32_t XOrShift32(uint32_t *state) {
  uint32_t x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

float random_float(uint32_t *state) {
  return XOrShift32(state) / (RAND_MAX + 1.0);
}

float random_float_range(uint32_t *state, float min, float max) {
  return min + (max - min) * random_float(state);
}

float clamp(float v, float min, float max) {
  return (v < min) ? min : (v > max) ? max : v;
}
