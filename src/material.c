#include "material.h"
#include "ray.h"
#include "util.h"

#include <cglm/vec4-ext.h>
#include <cglm/vec4.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void random_unit_sphere(uint32_t state[4], vec4 v) {
  while (true) {
    random_float_rangex4(state, -1.0, 1.0, v);
    v[3] = 0.0; // discard unwanted component
    if (glm_vec4_norm2(v) < 1.0) {
      return;
    }
  }
}

bool scatter_lambertian(Material material, Ray ray, struct HitRecord rec,
                        uint32_t state[4], color attenuation, Ray *scattered) {
  (void)ray;
  vec4 scatter_direction;
  vec4 ray_in_sphere;
  random_unit_sphere(state, ray_in_sphere);
  glm_vec4_add(rec.normal, ray_in_sphere, scatter_direction);

  // fprintf(stderr, "(%f, %f, %f, %f)\n", ray_in_sphere[0], ray_in_sphere[1],
  //         ray_in_sphere[2], ray_in_sphere[3]);

  if (glm_vec4_eq_eps(scatter_direction, 0.0)) {
    glm_vec4_copy(rec.normal, scatter_direction);
  }

  *scattered = ray_new(rec.p, scatter_direction);
  glm_vec4_copy(material.m_albedo, attenuation);
  return true;
}

// bool scatter_metal(const Material *material, const Ray *ray,
//                    const struct HitRecord rec, Color *attenuation,
//                    Ray *scattered) {
//   Vec3 reflected = ray->dir;
//   vec3_unit_vector(&reflected);
//   vec3_reflect(&reflected, &rec.normal);
//   Vec3 fuzz_vec = vec3_random_in_unit_sphere();
//   vec3_mul_scalar(&fuzz_vec, material->fuzz);
//   vec3_add(&reflected, &fuzz_vec);
//   *scattered = ray_new(rec.p, reflected);
//   *attenuation = material->m_albedo;
//   return vec3_dot(&scattered->dir, &rec.normal) > 0.0;
// }
//
// static double reflectance(double cosine, double ref_idx) {
//   double r0 = (1 - ref_idx) / (1 + ref_idx);
//   r0 = r0 * r0;
//   return r0 + (1 - r0) * pow(1 - cosine, 5);
// }
//
// bool scatter_dielectric(const Material *material, const Ray *ray,
//                         const struct HitRecord rec, Color *attenuation,
//                         Ray *scattered) {
//   *attenuation = vec3_new(1.0, 1.0, 1.0);
//
//   double refrection_ratio = rec.front_face ? 1.0 / material->ir :
//   material->ir;
//
//   Vec3 unit_dir = ray->dir;
//   vec3_unit_vector(&unit_dir);
//
//   Vec3 neg_unit_dir = unit_dir;
//   vec3_neg(&neg_unit_dir);
//   double cos_theta = fmin(vec3_dot(&neg_unit_dir, &rec.normal), 1.0);
//   double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
//
//   bool cannot_refract = refrection_ratio * sin_theta > 1.0;
//   Vec3 refrac_dir = unit_dir;
//
//   if (cannot_refract ||
//       reflectance(cos_theta, refrection_ratio) > random_double()) {
//     vec3_reflect(&refrac_dir, &rec.normal);
//   } else {
//     vec3_refract(&refrac_dir, &rec.normal, refrection_ratio);
//   }
//
//   *scattered = ray_new(rec.p, refrac_dir);
//   return true;
// }

bool material_scatter(Material material, Ray ray, const struct HitRecord rec,
                      uint32_t state[4], color attenuation, Ray *scattered) {
  switch (material.type) {
  case MATERIAL_TYPE_lambertian:
    scatter_lambertian(material, ray, rec, state, attenuation, scattered);
    break;
  case MATERIAL_TYPE_metal:
    // scatter_metal(material, ray, rec, attenuation, scattered);
  case MATERIAL_TYPE_dielectric:
    // scatter_dielectric(material, ray, rec, attenuation, scattered);
  default:
    fprintf(stderr, "ILLEGAL TYPE CALL IN get_scatter_action\n");
    exit(1);
  }
  return true;
}
