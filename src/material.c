#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <stdio.h>
#include <stdlib.h>

bool material_scatter(const Material *material, const Ray ray,
                      const struct HitRecord rec, Color *attenuation,
                      Ray *scattered) {
  Vec3 scatter_direction = vec3_add(rec.normal, vec3_random_unit_sphere());
  *scattered = ray_new(rec.p, scatter_direction);
  // TODO: make this less messy
  switch (material->type) {
  case MATERIAL_TYPE_lambertian:
    *attenuation = material->albedo;
    break;
  default:
    fprintf(stderr, "INVALID MATERIAL TYPE IN material_scatter!\n");
    exit(1);
  }
  return true;
}
