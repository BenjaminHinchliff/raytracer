#include "triangle.h"

bool triangle_hit(Triangle tri, Ray ray, double t_min, double t_max,
                  const Material *material, HitRecord *rec) {
  // ray and plane are parallel (approx)
  float NdotDir = glm_vec4_dot(tri.normal, ray.dir);
  if (fabs(NdotDir) < TRIANGLE_PARALLEL_EPSILON) {
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
  rec->material = material;

  return true;
}
