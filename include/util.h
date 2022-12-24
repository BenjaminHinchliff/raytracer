#ifndef UTIL_H
#define UTIL_H

#include <cglm/types.h>
#include <stdint.h>
#include <stdlib.h>

float degrees_to_radians(float degrees);

float random_float(uint32_t *state);
void random_floatx4(uint32_t state[4], vec4 out);
void random_float_rangex4(uint32_t state[4], float min, float max, vec4 out);

float clamp(float v, float min, float max);

#endif
