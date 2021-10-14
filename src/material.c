#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <stdio.h>
#include <stdlib.h>

typedef bool (*scatter_fn)(const Material *material, const Ray *ray,
                           const struct HitRecord rec, Color *attenuation,
                           Ray *scattered);

bool scatter_lambertian(const Material *material, const Ray *ray,
                        const struct HitRecord rec, Color *attenuation,
                        Ray *scattered) {
  Vec3 scatter_direction = rec.normal;
  Vec3 random_unit = vec3_random_in_unit_sphere();
  vec3_add(&scatter_direction, &random_unit);

  if (vec3_near_zero(&scatter_direction)) {
    scatter_direction = rec.normal;
  }

  *scattered = ray_new(rec.p, scatter_direction);
  *attenuation = material->albedo;
  return true;
}

bool scatter_panic(const Material *material, const Ray *ray,
                   const struct HitRecord rec, Color *attenuation,
                   Ray *scattered) {
  fprintf(stderr, "ILLEGAL TYPE CALL IN get_scatter_action\n");
  exit(1);
}

scatter_fn get_scatter_action(enum MATERIAL_TYPE type) {
  return (type == MATERIAL_TYPE_lambertian) ? scatter_lambertian
                                            : scatter_panic;
}

bool material_scatter(const Material *material, const Ray *ray,
                      const struct HitRecord rec, Color *attenuation,
                      Ray *scattered) {
  get_scatter_action(material->type)(material, ray, rec, attenuation,
                                     scattered);
  return true;
}
