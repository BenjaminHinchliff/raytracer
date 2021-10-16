#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "color.h"
#include "hit_record.h"
#include "ray.h"

#ifdef __cplusplus
extern "C" {
#endif

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
      Color l_albedo;
    };
    struct { // type = metal
      Color m_albedo;
      double fuzz;
    };
    struct { // type = dielectric
      double ir;
    };
  };
} Material;

bool material_scatter(const Material *material, const Ray *ray,
                      const struct HitRecord rec, Color *attenuation,
                      Ray *scattered);

#ifdef __cplusplus
}
#endif

#endif
