#include "aabb.h"

#include <cglm/util.h>

bool aabb_hit(AABB aabb, Ray ray, double t_min, double t_max) {
  for (int a = 0; a < 3; a++) {
    float invD = 1.0f / ray.dir[a];
    float t0 = (aabb.min[a] - ray.orig[a]) * invD;
    float t1 = (aabb.max[a] - ray.orig[a]) * invD;
    if (invD < 0.0f) {
      glm_swapf(&t0, &t1);
    }
    t_min = (t0 > t_min) ? t0 : t_min;
    t_max = (t1 < t_max) ? t1 : t_max;
    if (t_max <= t_min) {
      return false;
    }
  }
  return true;
}
