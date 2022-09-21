#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

float degrees_to_radians(float degrees);

float random_float(uint32_t *state);
float random_float_range(uint32_t *state, float min, float max);

float clamp(float v, float min, float max);

#ifdef __cplusplus
}
#endif

#endif
