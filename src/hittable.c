#include "hittable.h"
#include "ray.h"

#include <cglm/vec4.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void hit_record_set_face_normal(HitRecord *rec, Ray ray) {
  rec->front_face = glm_vec4_dot(ray.dir, rec->normal) < 0.0;
  if (!rec->front_face) {
    glm_vec4_negate(rec->normal);
  }
}

bool hittable_hit_sphere(Hittable sphere, Ray ray, double t_min, double t_max,
                         HitRecord *rec) {
  vec4 oc;
  glm_vec4_sub(ray.orig, sphere.center, oc);
  double a = glm_vec4_norm2(ray.dir);
  double half_b = glm_vec4_dot(oc, ray.dir);
  double c = glm_vec4_norm2(oc) - sphere.radius * sphere.radius;

  double discriminant = half_b * half_b - a * c;
  if (discriminant < 0.0) {
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
  ray_at(ray, rec->t, rec->p);
  glm_vec4_sub(rec->p, sphere.center, rec->normal);
  glm_vec4_divs(rec->normal, sphere.radius, rec->normal);
  hit_record_set_face_normal(rec, ray);

  rec->material = sphere.material;
  return true;
}

bool hittable_hit(Hittable hittable, Ray ray, double t_min, double t_max,
                  HitRecord *rec) {
  switch (hittable.type) {
  case HITTABLE_TYPE_sphere:
    return hittable_hit_sphere(hittable, ray, t_min, t_max, rec);
    break;
  default:
    fprintf(stderr, "ILLEGAL TYPE CALL IN get_hittable_action\n");
    exit(1);
  }
}

bool hittable_hit_multiple(World *hittables, Ray ray, double t_min,
                           double t_max, HitRecord *rec) {
  HitRecord tmp_rec;
  bool hit = false;
  double closest = t_max;

  for (size_t i = 0; i < hittables->num_objects; i++) {
    if (hittable_hit(hittables->objects[i], ray, t_min, closest, &tmp_rec)) {
      hit = true;
      closest = tmp_rec.t;
      *rec = tmp_rec;
    }
  }

  return hit;
}
