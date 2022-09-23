#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "color.h"
#include "ray.h"

enum MATERIAL_TYPE {
  MATERIAL_TYPE_lambertian,
  MATERIAL_TYPE_metal,
  MATERIAL_TYPE_dielectric,
};
#define NUM_MATERIAL_TYPES (1)

typedef struct Material {
  enum MATERIAL_TYPE type;
  union {
    struct { // type = lambertian
      color l_albedo;
    };
    struct { // type = metal
      color m_albedo;
      double fuzz;
    };
    struct { // type = dielectric
      double ir;
    };
  };
} Material;

typedef struct HitRecord {
  vec4 p;
  vec4 normal;
  Material material;
  double t;
  bool front_face;
} HitRecord;

bool material_scatter(Material material, Ray ray, const struct HitRecord rec,
                      uint32_t state[4], color attenuation, Ray *scattered);

#endif
