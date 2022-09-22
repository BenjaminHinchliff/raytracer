#include "trace.h"
#include "hittable.h"
#include "material.h"
#include "util.h"

#include <cglm/vec4.h>
#include <math.h>

void trace_ray(World *world, Ray ray, int max_depth, uint32_t state[4],
               color color_out) {
  color attenuation = GLM_VEC4_ONE_INIT;
  Ray scattered = ray;
  for (int i = 0; i < max_depth; i++) {
    HitRecord rec;
    bool hit = hittable_hit_multiple(world, scattered, 0.001, INFINITY, &rec);
    if (hit) {
      color mat_attenuation;
      if (material_scatter(rec.material, scattered, rec, state, mat_attenuation,
                           &scattered)) {
        glm_vec4_mul(attenuation, mat_attenuation, attenuation);
      }
    } else {
      vec4 unit_dir;
      glm_vec4_normalize_to(ray.dir, unit_dir);
      double t = 0.5 * (unit_dir[1] + 1.0);
      color bg1 = {1.0, 1.0, 1.0, 1.0};
      color bg2 = {0.5, 0.7, 1.0, 1.0};
      color lerped;
      glm_vec4_lerp(bg1, bg2, t, lerped);
      glm_vec4_mul(attenuation, lerped, attenuation);
      break;
    }
  }

  glm_vec4_copy(attenuation, color_out);
}

void trace_rows(World *world, uint32_t state[4], int start, int end,
                int samples, int max_depth, Image *image) {
  for (int y = start; y < end; y++) {
    for (int x = 0; x < image->width; x++) {
      color sample = GLM_VEC4_ZERO_INIT;
      for (int s = 0; s < samples; s++) {
        vec4 floats;
        random_floatx4(state, floats);
        float u = ((float)x + floats[0]) / (float)(image->width - 1);
        float v = ((float)y + floats[1]) / (float)(image->height - 1);
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
