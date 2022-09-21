#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

float degrees_to_radians(float degrees);

float random_float();
float random_float_range(float min, float max);

float clamp(float v, float min, float max);

#ifdef __cplusplus
}
#endif

#endif
