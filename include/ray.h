#ifndef RAY_H
#define RAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vec3.h"

typedef struct Ray {
  Point3 orig;
  Vec3 dir;
} Ray;

Ray ray_new(Point3 orig, Vec3 dir);

Point3 ray_at(const Ray *ray, double t);

#ifdef __cplusplus
}
#endif

#endif
