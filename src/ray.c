#include "ray.h"
#include "vec3.h"

point3 ray_at(Ray ray, double t) {
  return vec3_add(ray.orig, vec3_mul_scalar(ray.dir, t));
}
