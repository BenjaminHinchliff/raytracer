#include "material.h"
#include "ray.h"
#include "util.h"

#include <cglm/vec3.h>
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

  if (glm_vec4_eq_eps(scatter_direction, 0.0)) {
    glm_vec4_copy(rec.normal, scatter_direction);
  }

  *scattered = ray_new(rec.p, scatter_direction);
  glm_vec4_copy(material.m_albedo, attenuation);
  return true;
}

void reflect(vec4 v, vec4 n, vec4 res) {
  float dot = glm_vec4_dot(v, n);
  glm_vec4_scale(n, 2 * dot, res);
  glm_vec4_sub(v, res, res);
}

bool scatter_metal(Material material, Ray ray, struct HitRecord rec,
                   uint32_t state[4], color attenuation, Ray *scattered) {
  vec4 reflected;
  reflect(ray.dir, rec.normal, reflected);
  vec4 fuzz_vec;
  random_unit_sphere(state, fuzz_vec);
  glm_vec4_muladds(fuzz_vec, material.fuzz, reflected);
  *scattered = ray_new(rec.p, reflected);
  glm_vec4_copy(material.m_albedo, attenuation);
  return glm_vec4_dot(scattered->dir, rec.normal) > 0.0;
}

void refract(vec4 uv, vec4 n, double refraction_ratio, vec4 res) {
  vec4 neg_uv;
  glm_vec4_negate_to(uv, neg_uv);
  float cos_theta = fminf(glm_vec4_dot(neg_uv, n), 1.0);

  vec4 r_out_perp;
  glm_vec4_scale(n, cos_theta, r_out_perp);
  glm_vec4_add(r_out_perp, uv, r_out_perp);
  glm_vec4_scale(r_out_perp, refraction_ratio, r_out_perp);

  vec4 r_out_parallel;
  glm_vec4_scale(n, -sqrtf(fabsf(1.0f - glm_vec4_norm2(r_out_perp))),
                 r_out_parallel);

  glm_vec4_add(r_out_perp, r_out_parallel, res);
}

static double reflectance(double cosine, double ref_idx) {
  double r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow(1 - cosine, 5);
}

bool scatter_dielectric(Material material, Ray ray, struct HitRecord rec,
                        uint32_t state[4], color attenuation, Ray *scattered) {
  glm_vec4_one(attenuation);

  double refrection_ratio = rec.front_face ? 1.0 / material.ir : material.ir;

  vec4 unit_dir;
  glm_vec3_normalize_to(ray.dir, unit_dir);

  vec4 neg_unit_dir;
  glm_vec4_negate_to(unit_dir, neg_unit_dir);
  double cos_theta = fmin(glm_vec4_dot(neg_unit_dir, rec.normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = refrection_ratio * sin_theta > 1.0;
  vec4 refrac_dir;

  if (cannot_refract ||
      reflectance(cos_theta, refrection_ratio) > random_float(state)) {
    reflect(unit_dir, rec.normal, refrac_dir);
  } else {
    refract(unit_dir, rec.normal, refrection_ratio, refrac_dir);
  }

  *scattered = ray_new(rec.p, refrac_dir);
  return true;
}

bool material_scatter(Material material, Ray ray, struct HitRecord rec,
                      uint32_t state[4], color attenuation, Ray *scattered) {
  switch (material.type) {
  case MATERIAL_TYPE_lambertian:
    scatter_lambertian(material, ray, rec, state, attenuation, scattered);
    break;
  case MATERIAL_TYPE_metal:
    scatter_metal(material, ray, rec, state, attenuation, scattered);
    break;
  case MATERIAL_TYPE_dielectric:
    scatter_dielectric(material, ray, rec, state, attenuation, scattered);
    break;
  default:
    fprintf(stderr, "ILLEGAL TYPE CALL IN get_scatter_action\n");
    exit(1);
  }
  return true;
}
