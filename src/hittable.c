#include "hittable.h"
#include "hit_record.h"
#include "ray.h"
#include "world.h"

#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1e-6

bool hittable_hit_triangle(Hittable tri, Ray ray, double t_min, double t_max,
                           HitRecord *rec) {
  // ray and plane are parallel (approx)
  float NdotDir = glm_vec4_dot(tri.normal, ray.dir);
  if (fabs(NdotDir) < EPSILON) {
    return false;
  }

  float d = -glm_vec4_dot(tri.normal, tri.v0);
  float t = -(glm_vec4_dot(tri.normal, ray.orig) + d) / NdotDir;

  // in bounds
  if (t < t_min || t > t_max) {
    return false;
  }

  // get ray at point
  vec4 p;
  ray_at(ray, t, p);

  // vector perpendicular to triangle plane
  vec4 C = GLM_VEC4_ZERO_INIT;

  vec4 edge0;
  glm_vec4_sub(tri.v1, tri.v0, edge0);
  vec4 vp0;
  glm_vec4_sub(p, tri.v0, vp0);

  glm_vec3_cross(edge0, vp0, C);
  if (glm_vec4_dot(tri.normal, C) < 0.0f) {
    return false;
  }

  vec4 edge1;
  glm_vec4_sub(tri.v2, tri.v1, edge1);
  vec4 vp1;
  glm_vec4_sub(p, tri.v1, vp1);

  glm_vec3_cross(edge1, vp1, C);
  if (glm_vec4_dot(tri.normal, C) < 0.0f) {
    return false;
  }

  vec4 edge2;
  glm_vec4_sub(tri.v0, tri.v2, edge2);
  vec4 vp2;
  glm_vec4_sub(p, tri.v2, vp2);

  glm_vec3_cross(edge2, vp2, C);
  if (glm_vec4_dot(tri.normal, C) < 0.0f) {
    return false;
  }

  rec->t = t;
  glm_vec4_copy(p, rec->p);
  glm_vec4_copy(tri.normal, rec->normal);
  hit_record_set_face_normal(rec, ray);
  rec->material = tri.material;

  return true;
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
  case HITTABLE_TYPE_triangle:
    return hittable_hit_triangle(hittable, ray, t_min, t_max, rec);
    break;
  default:
    fprintf(stderr, "ILLEGAL TYPE CALL IN get_hittable_action: %d\n",
            hittable.type);
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
