#include "ray.h"
#include "vec3.h"

Ray ray_new(Point3 orig, Vec3 dir) {
  return (Ray){
      .orig = orig,
      .dir = dir,
  };
}

Point3 ray_at(Ray ray, double t) {
  return vec3_add(ray.orig, vec3_mul_scalar(ray.dir, t));
}
