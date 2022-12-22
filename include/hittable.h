#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "material.h"
#include "model.h"
#include "ray.h"
#include "triangle.h"

#include <cglm/vec4.h>

typedef struct World World;

enum HITTABLE_TYPE {
  HITTABLE_TYPE_sphere,
  HITTABLE_TYPE_triangle,
  HITTABLE_TYPE_model,
};
#define NUM_HITTABLE_TYPES 3

typedef struct Hittable {
  enum HITTABLE_TYPE type;
  union {
    struct { // type = sphere
      vec4 center;
      double radius;
    };
    struct { // type = triangle
      Triangle tri;
    };
    struct { // type = model
      Model model;
    };
  };
  const Material *material;
} Hittable;

bool hittable_hit(Hittable hittable, Ray ray, double t_min, double t_max,
                  HitRecord *rec);

bool hittable_hit_multiple(World *hittables, Ray ray, double t_min,
                           double t_max, HitRecord *rec);

#endif
