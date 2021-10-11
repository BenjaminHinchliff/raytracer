#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct Ray {
  Point3 orig;
  Vec3 dir;
} Ray;

Point3 ray_at(Ray ray, double t);

#endif
