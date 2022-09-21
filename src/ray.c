#include "ray.h"
#include <cglm/vec3.h>

Ray ray_new(vec4 orig, vec4 dir) {
  Ray ray;
  glm_vec4_copy(orig, ray.orig);
  glm_vec4_copy(dir, ray.dir);
  return ray;
}

void ray_at(Ray ray, double t, vec4 at) {
  glm_vec4_copy(ray.dir, at);
  glm_vec4_scale(at, t, at);
  glm_vec4_add(at, ray.orig, at);
}
