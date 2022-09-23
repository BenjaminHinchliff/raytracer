#include "util.h"

float degrees_to_radians(float degrees) { return degrees * M_PI / 180.0; }

uint32_t XOrShift32(uint32_t *state) {
  // hoping this can be auto vectorized
  uint32_t x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

void XOrShift32x4(uint32_t state[4]) {
  // hoping this can be auto vectorized
  for (int i = 0; i < 4; i++) {
    state[i] ^= state[i] << 13;
    state[i] ^= state[i] >> 17;
    state[i] ^= state[i] << 5;
  }
}

void random_floatx4(uint32_t state[4], vec4 out) {
  XOrShift32x4(state);
  for (int i = 0; i < 4; i++) {
    out[i] = state[i] / (UINT32_MAX + 1.0);
  }
}

void random_float_rangex4(uint32_t state[4], float min, float max, vec4 out) {
  random_floatx4(state, out);
  for (int i = 0; i < 4; i++) {
    out[i] = min + (max - min) * out[i];
  }
}

float clamp(float v, float min, float max) {
  return (v < min) ? min : (v > max) ? max : v;
}
