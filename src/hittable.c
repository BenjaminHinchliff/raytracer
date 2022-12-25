#include "hittable.h"
#include "hit_record.h"
#include "hittable_list.h"
#include "model.h"
#include "ray.h"
#include "triangle.h"
#include "world.h"

#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
  case HITTABLE_TYPE_triangle:
    return triangle_hit(hittable.tri, ray, t_min, t_max, hittable.material,
                        rec);
  case HITTABLE_TYPE_model:
    return model_hit(hittable.model, ray, t_min, t_max, hittable.material, rec);
  case HITTABLE_TYPE_list:
    return hittable_list_hit(*hittable.list, ray, t_min, t_max, rec);
  default:
    fprintf(stderr, "ILLEGAL TYPE CALL IN get_hittable_action: %d\n",
            hittable.type);
    exit(1);
  }
}

bool hittable_bounding_box(Hittable hittable, AABB *box) {
  switch (hittable.type) {
  case HITTABLE_TYPE_sphere: {
    float radius = hittable.radius;
    vec4 r_vec = {radius, radius, radius, 0.0f};
    glm_vec4_sub(hittable.center, r_vec, box->min);
    glm_vec4_add(hittable.center, r_vec, box->max);
    return true;
  }
  default:
    fprintf(stderr, "unknown hittable for bounding box generation\n");
    exit(1);
  }
}
