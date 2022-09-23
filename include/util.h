#ifndef UTIL_H
#define UTIL_H

#include <cglm/types.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

float degrees_to_radians(float degrees);

void random_floatx4(uint32_t state[4], vec4 out);
void random_float_rangex4(uint32_t state[4], float min, float max, vec4 out);

float clamp(float v, float min, float max);

#ifdef __cplusplus
}
#endif

#endif
