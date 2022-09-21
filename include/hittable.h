#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "material.h"
#include "ray.h"

#include <cglm/vec4.h>

#ifdef __cplusplus
extern "C" {
#endif

enum HITTABLE_TYPE {
  HITTABLE_TYPE_sphere,
};
#define NUM_HITTABLE_TYPES 1

typedef struct Hittable {
  enum HITTABLE_TYPE type;
  union {
    struct { // type = sphere
      vec4 center;
      double radius;
      Material material;
    };
  };
} Hittable;

bool hittable_hit(Hittable hittable, Ray ray, double t_min, double t_max,
                  HitRecord *rec);

bool hittable_hit_multiple(Hittable *hittables, size_t num, Ray ray,
                           double t_min, double t_max, HitRecord *rec);

#ifdef __cplusplus
}
#endif

#endif
