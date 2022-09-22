#include "trace.h"
#include "hittable.h"
#include "material.h"
#include "util.h"

void trace_ray(World *world, Ray ray, int depth, uint32_t *state,
               color color_out) {
  if (depth == 0) {
    glm_vec4_zero(color_out);
    return;
  }

  HitRecord rec;
  bool hit = hittable_hit_multiple(world, ray, 0.001, INFINITY, &rec);
  if (hit) {
    Ray scattered;
    color attenuation;
    if (material_scatter(rec.material, ray, rec, state, attenuation,
                         &scattered)) {
      color color;
      trace_ray(world, scattered, depth - 1, state, color);
      glm_vec4_mul(color, attenuation, color);
      glm_vec4_copy(color, color_out);
      return;
    }
    glm_vec4_zero(color_out);
    return;
  }

  vec4 unit_dir;
  glm_vec4_normalize_to(ray.dir, unit_dir);
  double t = 0.5 * (unit_dir[1] + 1.0);
  color bg1 = {1.0, 1.0, 1.0, 1.0};
  color bg2 = {0.5, 0.7, 1.0, 1.0};
  glm_vec4_lerp(bg1, bg2, t, color_out);
  return;
}

void trace_rows(World *world, uint32_t *state, int start, int end, int samples,
                int max_depth, Image *image) {
  for (int y = start; y < end; y++) {
    for (int x = 0; x < image->width; x++) {
      color sample = GLM_VEC4_ZERO_INIT;
      for (int s = 0; s < samples; s++) {
        float u = ((float)x + random_float(state)) / (float)(image->width - 1);
        float v = ((float)y + random_float(state)) / (float)(image->height - 1);
        Ray ray = camera_get_ray(world->camera, u, v);
        color next_color;
        trace_ray(world, ray, max_depth, state, next_color);
        glm_vec4_add(sample, next_color, sample);
      }
      glm_vec4_divs(sample, samples, sample);

      uint8_t r, g, b;
      vec4_to_color(sample, &r, &g, &b);

      image_set_pixel(image, x, image->height - y - 1, r, g, b);
    }
  }
}
