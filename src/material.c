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
  Vec3 random_unit = vec3_random_unit_sphere();
  vec3_add(&scatter_direction, &random_unit);

  if (vec3_near_zero(&scatter_direction)) {
    scatter_direction = rec.normal;
  }

  *scattered = ray_new(rec.p, scatter_direction);
  *attenuation = material->m_albedo;
  return true;
}

bool scatter_metal(const Material *material, const Ray *ray,
                   const struct HitRecord rec, Color *attenuation,
                   Ray *scattered) {
  Vec3 reflected = ray->dir;
  vec3_unit_vector(&reflected);
  vec3_reflect(&reflected, &rec.normal);
  Vec3 fuzz_vec = vec3_random_in_unit_sphere();
  vec3_mul_scalar(&fuzz_vec, material->fuzz);
  vec3_add(&reflected, &fuzz_vec);
  *scattered = ray_new(rec.p, reflected);
  *attenuation = material->m_albedo;
  return vec3_dot(&scattered->dir, &rec.normal) > 0.0;
}

bool scatter_dielectric(const Material *material, const Ray *ray,
                        const struct HitRecord rec, Color *attenuation,
                        Ray *scattered) {
  *attenuation = vec3_new(1.0, 1.0, 1.0);

  double refrection_ratio = rec.front_face ? 1.0 / material->ir : material->ir;

  Vec3 refracted = ray->dir;
  vec3_unit_vector(&refracted);
  vec3_refract(&refracted, &rec.normal, refrection_ratio);

  *scattered = ray_new(rec.p, refracted);
  return true;
}

bool scatter_panic(const Material *material, const Ray *ray,
                   const struct HitRecord rec, Color *attenuation,
                   Ray *scattered) {
  fprintf(stderr, "ILLEGAL TYPE CALL IN get_scatter_action\n");
  exit(1);
}

scatter_fn get_scatter_action(enum MATERIAL_TYPE type) {
  return (type == MATERIAL_TYPE_lambertian)   ? scatter_lambertian
         : (type == MATERIAL_TYPE_metal)      ? scatter_metal
         : (type == MATERIAL_TYPE_dielectric) ? scatter_dielectric
                                              : scatter_panic;
}

bool material_scatter(const Material *material, const Ray *ray,
                      const struct HitRecord rec, Color *attenuation,
                      Ray *scattered) {
  get_scatter_action(material->type)(material, ray, rec, attenuation,
                                     scattered);
  return true;
}
