#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool (*hittable_fn)(Hittable sphere, const Ray ray, double t_min,
                            double t_max, HitRecord *rec);

bool hittable_hit_sphere(Hittable sphere, const Ray ray, double t_min,
                         double t_max, HitRecord *rec) {
  Vec3 oc = vec3_sub(ray.orig, sphere.center);
  double a = vec3_length_squared(ray.dir);
  double half_b = vec3_dot(oc, ray.dir);
  double c = vec3_length_squared(oc) - sphere.radius * sphere.radius;

  double discriminant = half_b * half_b - a * c;
  if (discriminant < 0) {
    return false;
  }

  double sqrt_dis = sqrt(discriminant);

  double root = (-half_b - sqrt_dis) / a;
  if (root < t_min || root > t_max) {
    root = (-half_b + sqrt_dis) / a;
    if (root < t_min || root > t_max) {
      return false;
    }
  }

  rec->t = root;
  rec->p = ray_at(ray, rec->t);
  rec->normal = vec3_div_scalar(vec3_sub(rec->p, sphere.center), sphere.radius);

  return true;
}

bool hittable_hit_panic(Hittable sphere, const Ray ray, double t_min,
                        double t_max, HitRecord *rec) {
  fprintf(stderr, "ILLEGAL TYPE CALL IN get_hittable_action\n");
  exit(1);
}

hittable_fn get_hittable_action(enum HITTABLE_TYPE type) {
  return (type == HITTABLE_TYPE_sphere) ? hittable_hit_sphere
                                        : hittable_hit_panic;
}

bool hittable_hit(Hittable hittable, const Ray ray, double t_min, double t_max,
                  HitRecord *rec) {
  return get_hittable_action(hittable.type)(hittable, ray, t_min, t_max, rec);
}
