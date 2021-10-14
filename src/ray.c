#include "ray.h"
#include "vec3.h"

Ray ray_new(Point3 orig, Vec3 dir) {
  return (Ray){
      .orig = orig,
      .dir = dir,
  };
}

Point3 ray_at(const Ray *ray, double t) {
  Vec3 at = ray->dir;
  vec3_mul_scalar(&at, t);
  vec3_add(&at, &ray->orig);
  return at;
}
