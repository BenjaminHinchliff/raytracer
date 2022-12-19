#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "material.h"
#include "ray.h"

#include <cglm/vec4.h>

typedef struct World World;

enum HITTABLE_TYPE {
  HITTABLE_TYPE_sphere,
  HITTABLE_TYPE_triangle,
};
#define NUM_HITTABLE_TYPES 2

typedef struct Hittable {
  enum HITTABLE_TYPE type;
  union {
    struct { // type = sphere
      vec4 center;
      double radius;
    };
    struct { // type = triangle
      vec4 v0;
      vec4 v1;
      vec4 v2;
      vec4 normal;
    };
  };
  const Material *material;
} Hittable;

bool hittable_hit(Hittable hittable, Ray ray, double t_min, double t_max,
                  HitRecord *rec);

bool hittable_hit_multiple(World *hittables, Ray ray, double t_min,
                           double t_max, HitRecord *rec);

#endif
