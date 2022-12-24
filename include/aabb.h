#ifndef AABB_H
#define AABB_H

#include "ray.h"

#include <cglm/vec4.h>

typedef struct AABB {
  vec4 min;
  vec4 max;
} AABB;

bool aabb_hit(AABB aabb, Ray ray, double t_min, double t_max);

#endif
