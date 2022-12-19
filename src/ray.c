#include "ray.h"

#include <cglm/vec4.h>

Ray ray_new(vec4 orig, vec4 dir) {
  Ray ray;
  glm_vec4_copy(orig, ray.orig);
  glm_vec4_copy(dir, ray.dir);
  glm_vec4_normalize(ray.dir);
  return ray;
}

void ray_at(Ray ray, double t, vec4 at) {
  glm_vec4_copy(ray.orig, at);
  glm_vec4_muladds(ray.dir, t, at);
}
