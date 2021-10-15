#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 400;
const int image_height = (int)(image_width / aspect_ratio);
const int num_channels = 3;
const int samples_per_pixel = 100;
const int max_depth = 50;

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass);

Color ray_color(const Ray *ray, const Hittable *world, const size_t world_len,
                int depth);

int main(int argc, char **argv) {
  const Hittable world[] = {{.type = HITTABLE_TYPE_sphere,
                             .center = vec3_new(0.0, 0.0, -1.0),
                             .radius = 0.5,
                             .material =
                                 {
                                     .type = MATERIAL_TYPE_lambertian,
                                     .albedo = vec3_new(0.7, 0.3, 0.3),
                                 }},
                            {.type = HITTABLE_TYPE_sphere,
                             .center = vec3_new(-1.0, 0.0, -1.0),
                             .radius = 0.5,
                             .material =
                                 {
                                     .type = MATERIAL_TYPE_metal,
                                     .albedo = vec3_new(0.8, 0.8, 0.8),
                                 }},
                            {.type = HITTABLE_TYPE_sphere,
                             .center = vec3_new(1.0, 0.0, -1.0),
                             .radius = 0.5,
                             .material =
                                 {
                                     .type = MATERIAL_TYPE_metal,
                                     .albedo = vec3_new(0.8, 0.6, 0.2),
                                 }},
                            {.type = HITTABLE_TYPE_sphere,
                             .center = vec3_new(0.0, -100.5, -1.0),
                             .radius = 100,
                             .material = {
                                 .type = MATERIAL_TYPE_lambertian,
                                 .albedo = vec3_new(0.8, 0.8, 0.0),
                             }}};
  const size_t world_len = sizeof(world) / sizeof(Hittable);

  Camera camera = camera_new(aspect_ratio);

  png_bytepp image = malloc(sizeof(png_bytep) * image_height);
  for (size_t i = 0; i < image_height; i += 1) {
    image[i] = malloc(sizeof(png_byte) * num_channels * image_width);
  }

  for (int i = image_height - 1; i >= 0; i -= 1) {
    fprintf(stderr, "\rScanlines remaining: %d ", i);
    for (int j = 0; j < image_width; j += 1) {
      Color color = vec3_origin();
      for (int s = 0; s < samples_per_pixel; s += 1) {
        double u = ((double)j + random_double()) / (double)(image_width - 1);
        double v = ((double)i + random_double()) / (double)(image_height - 1);
        Ray ray = camera_get_ray(&camera, u, v);
        Color next_color = ray_color(&ray, world, world_len, max_depth);
        vec3_add(&color, &next_color);
      }
      vec3_div_scalar(&color, samples_per_pixel);

      uint8_t r, g, b;
      vec3_to_color(&color, &r, &g, &b);

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

  for (size_t i = 0; i < image_height; i += 1) {
    free(image[i]);
  }
  free(image);
  fprintf(stderr, "\nDone!\n");

  return 0;
}

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass) {
  fprintf(stderr, "\rPng write rows remaining: %d ", image_height - row);
}

Color ray_color(const Ray *ray, const Hittable *world, const size_t world_len,
                int depth) {
  if (depth == 0) {
    return vec3_origin();
  }

  HitRecord rec;
  bool hit =
      hittable_hit_multiple(world, world_len, ray, 0.001, INFINITY, &rec);
  if (hit) {
    Ray scattered;
    Color attenuation;
    if (material_scatter(rec.material, ray, rec, &attenuation, &scattered)) {
      Color color = ray_color(&scattered, world, world_len, depth - 1);
      vec3_mul(&color, &attenuation);
      return color;
    }
    return vec3_origin();
  }

  Vec3 unit_dir = ray->dir;
  vec3_unit_vector(&unit_dir);
  double t = 0.5 * (unit_dir.y + 1.0);
  Color color = vec3_new(1.0, 1.0, 1.0);
  Color target = vec3_new(0.5, 0.7, 1.0);
  vec3_lerp(&color, &target, t);
  return color;
}
