#include "trace.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "util.h"

#include <cglm/vec4.h>
#include <math.h>
#include <stdio.h>

void trace_ray(World *world, Ray ray, uint32_t state[4], color result) {
  color attenuation = GLM_VEC4_ONE_INIT;
  Ray scattered = ray;
  unsigned int depth;
  for (depth = 0; depth < world->screen.max_depth; depth++) {
    HitRecord rec;
    bool hit = hittable_hit_multiple(world, scattered, 0.001, INFINITY, &rec);
    if (hit) {
      color mat_attenuation;
      if (material_scatter(rec.material, scattered, rec, state, mat_attenuation,
                           &scattered)) {
        glm_vec4_mul(attenuation, mat_attenuation, attenuation);
      } else {
        glm_vec4_zero(attenuation);
        break;
      }
    } else {
      vec4 unit_dir;
      glm_vec4_normalize_to(ray.dir, unit_dir);
      double t = 0.5 * (unit_dir[1] + 1.0);
      color bg1 = {1.0, 1.0, 1.0, 1.0};
      color bg2 = {0.5, 0.7, 1.0, 1.0};
      color bg;
      glm_vec4_lerp(bg1, bg2, t, bg);
      glm_vec4_mul(attenuation, bg, attenuation);
      break;
    }
  }

  if (depth == world->screen.max_depth) {
    glm_vec4_zero(attenuation);
  }

  glm_vec4_copy(attenuation, result);
}

Image *trace_rows(World *world, uint32_t state[4]) {
  Image *image = image_new(world->screen.width, world->screen.height);

  const int chunk = 16;
#pragma omp parallel shared(world, image)
  {
#pragma omp for schedule(static, chunk)
    for (int y = 0; y < image->height; y++) {
      for (int x = 0; x < image->width; x++) {
        color sample = GLM_VEC4_ZERO_INIT;
        for (unsigned int s = 0; s < world->screen.samples; s++) {
          vec4 floats;
          random_floatx4(state, floats);
          float u = ((float)x + floats[0]) / (float)(image->width - 1);
          float v = ((float)y + floats[1]) / (float)(image->height - 1);
          Ray ray = camera_get_ray(world->camera, u, v);
          color next_color;
          trace_ray(world, ray, state, next_color);
          glm_vec4_add(sample, next_color, sample);
        }
        glm_vec4_divs(sample, world->screen.samples, sample);

        uint8_t r, g, b;
        vec4_to_color(sample, &r, &g, &b);

        image_set_pixel(image, x, image->height - y - 1, r, g, b);
      }
    }
  }

  return image;
}
