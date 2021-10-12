#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

double degrees_to_radians(double degrees);

double random_double();
double random_double_range(double min, double max);

#ifdef __cplusplus
}
#endif

#endif
