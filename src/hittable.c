#include "hittable.h"
#include "ray.h"
#include "vec3.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

HitRecord hit_record_set_face_normal(HitRecord hit_record, const Ray ray,
                                     Vec3 outward_normal) {
  hit_record.front_face = vec3_dot(&ray.dir, &outward_normal);
  if (!hit_record.front_face) {
    vec3_neg(&outward_normal);
  }
  hit_record.normal = outward_normal;
  return hit_record;
}

typedef bool (*hittable_fn)(const Hittable *sphere, const Ray ray, double t_min,
                            double t_max, HitRecord *rec);

bool hittable_hit_sphere(const Hittable *sphere, const Ray ray, double t_min,
                         double t_max, HitRecord *rec) {
  Vec3 oc = ray.orig;
  vec3_sub(&oc, &sphere->center);
  double a = vec3_length_squared(&ray.dir);
  double half_b = vec3_dot(&oc, &ray.dir);
  double c = vec3_length_squared(&oc) - sphere->radius * sphere->radius;

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
  Vec3 outward_normal = rec->p;
  vec3_sub(&outward_normal, &sphere->center);
  vec3_div_scalar(&outward_normal, sphere->radius);
  *rec = hit_record_set_face_normal(*rec, ray, outward_normal);

  rec->material = &sphere->material;
  return true;
}

bool hittable_hit_panic(const Hittable *sphere, const Ray ray, double t_min,
                        double t_max, HitRecord *rec) {
  fprintf(stderr, "ILLEGAL TYPE CALL IN get_hittable_action\n");
  exit(1);
}

hittable_fn get_hittable_action(enum HITTABLE_TYPE type) {
  return (type == HITTABLE_TYPE_sphere) ? hittable_hit_sphere
                                        : hittable_hit_panic;
}

bool hittable_hit(const Hittable *hittable, const Ray ray, double t_min,
                  double t_max, HitRecord *rec) {
  return get_hittable_action(hittable->type)(hittable, ray, t_min, t_max, rec);
}

bool hittable_hit_multiple(const Hittable *hittables, size_t num, const Ray ray,
                           double t_min, double t_max, HitRecord *rec) {
  HitRecord tmp_rec;
  bool hit = false;
  double closest = t_max;

  for (size_t i = 0; i < num; i += 1) {
    if (hittable_hit(&hittables[i], ray, t_min, closest, &tmp_rec)) {
      hit = true;
      closest = tmp_rec.t;
      *rec = tmp_rec;
    }
  }

  return hit;
}
