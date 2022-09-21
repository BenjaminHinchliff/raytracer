#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "png.h"
#include "png_write.h"
#include "pngconf.h"
#include "ray.h"
#include "util.h"
#include "vec3.h"

#include <cglm/vec4.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = (int)(image_width / aspect_ratio);
const int num_channels = 3;
const int samples_per_pixel = 100;
const int max_depth = 20;

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass);

void ray_color(Ray ray, Hittable world[], const size_t world_len, int depth,
               color color_out);

int main(void) {
  // const Material mat_left = {
  //     .type = MATERIAL_TYPE_dielectric,
  //     .ir = 1.5,
  // };
  Hittable world[] = {
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {-1.0, 0.0, -1.0, 0.0},
      //     .radius = 0.5,
      //     .material = mat_left,
      // },
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {-1.0, 0.0, -1.0, 0.0},
      //     .radius = -0.4,
      //     .material = mat_left,
      // },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, 0.0, -1.0, 0.0},
          .radius = 0.5,
          .material =
              {
                  .type = MATERIAL_TYPE_lambertian,
                  .l_albedo = {0.1, 0.2, 0.5, 1.0},
              },
      },
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {1.0, 0.0, -1.0, 0.0},
      //     .radius = 0.5,
      //     .material =
      //         {
      //             .type = MATERIAL_TYPE_metal,
      //             .m_albedo = {0.8, 0.6, 0.2, 1.0},
      //             .fuzz = 0.3,
      //         },
      // },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, -100.5, -1.0, 0.0},
          .radius = 100,
          .material =
              {
                  .type = MATERIAL_TYPE_lambertian,
                  .l_albedo = {0.8, 0.8, 0.0, 1.0},
              },
      },
  };
  const size_t world_len = sizeof(world) / sizeof(Hittable);

  Camera camera = camera_new(aspect_ratio);

  png_bytepp image = malloc(sizeof(png_bytep) * image_height);
  for (int i = 0; i < image_height; i += 1) {
    image[i] = malloc(sizeof(png_byte) * num_channels * image_width);
  }

  for (int i = image_height - 1; i >= 0; i -= 1) {
    fprintf(stderr, "\rScanlines remaining: %d ", i);
    for (int j = 0; j < image_width; j += 1) {
      color sample = GLM_VEC4_ZERO_INIT;
      for (int s = 0; s < samples_per_pixel; s += 1) {
        float u = ((float)j + random_float()) / (float)(image_width - 1);
        float v = ((float)i + random_float()) / (float)(image_height - 1);
        Ray ray = camera_get_ray(camera, u, v);
        color next_color;
        ray_color(ray, world, world_len, max_depth, next_color);
        glm_vec4_add(sample, next_color, sample);
      }
      glm_vec4_divs(sample, samples_per_pixel, sample);

      uint8_t r, g, b;
      vec4_to_color(sample, &r, &g, &b);

      int y = image_height - i - 1;
      int x = j * num_channels;
      image[y][x] = r;
      image[y][x + 1] = g;
      image[y][x + 2] = b;
    }
  }
  fprintf(stderr, "\n");

  write_png_file("out.png", image_width, image_height, num_channels, image,
                 write_row_progress_callback);

  for (int i = 0; i < image_height; i += 1) {
    free(image[i]);
  }
  free(image);
  fprintf(stderr, "\nDone!\n");

  return 0;
}

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass) {
  (void)png_ptr, (void)pass;
  fprintf(stderr, "\rPng write rows remaining: %d ", image_height - row);
}

void ray_color(Ray ray, Hittable world[], const size_t world_len, int depth,
               color color_out) {
  if (depth == 0) {
    glm_vec4_zero(color_out);
    return;
  }

  HitRecord rec;
  bool hit =
      hittable_hit_multiple(world, world_len, ray, 0.001, INFINITY, &rec);
  if (hit) {
    Ray scattered;
    color attenuation;
    if (material_scatter(rec.material, ray, rec, attenuation, &scattered)) {
      color color;
      ray_color(scattered, world, world_len, depth - 1, color);
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
