#ifndef RAY_H
#define RAY_H

#include <cglm/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/vec3.h>

typedef struct Ray {
  vec4 orig;
  vec4 dir;
} Ray;

Ray ray_new(vec4 orig, vec4 dir);

void ray_at(Ray ray, double t, vec4 at);

#ifdef __cplusplus
}
#endif

#endif
