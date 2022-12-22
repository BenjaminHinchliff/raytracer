#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cglm/vec4.h>

#include "hit_record.h"

#ifndef TRIANGLE_PARALLEL_EPSILON
#define TRIANGLE_PARALLEL_EPSILON (1e-6)
#endif

typedef struct Triangle {
  vec4 v0;
  vec4 v1;
  vec4 v2;
  vec4 normal;
} Triangle;

bool triangle_hit(Triangle tri, Ray ray, double t_min, double t_max,
                  const Material *material, HitRecord *rec);

#endif
