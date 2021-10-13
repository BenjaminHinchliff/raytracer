#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "util.h"
#include "vec3.h"

Color ray_color(const Ray ray, const Hittable *world, const size_t world_len,
                int depth) {
  if (depth == 0) {
    return vec3_origin();
  }

  HitRecord rec;
  bool hit = hittable_hit_multiple(world, world_len, ray, 0, INFINITY, &rec);
  if (hit) {
    Point3 target =
        vec3_add(vec3_add(rec.p, rec.normal), vec3_random_in_unit_sphere());
    return vec3_mul_scalar(ray_color(ray_new(rec.p, vec3_sub(target, rec.p)),
                                     world, world_len, depth - 1),
                           0.5);
  }
  Vec3 unit_dir = vec3_unit_vector(ray.dir);
  double t = 0.5 * (unit_dir.y + 1.0);
  return vec3_lerp(vec3_new(1.0, 1.0, 1.0), vec3_new(0.5, 0.7, 1.0), t);
}

int main(int argc, char **argv) {
  const double aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = (int)(image_width / aspect_ratio);
  const int num_channels = 3;
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  const Hittable world[] = {(Hittable){
                                .type = HITTABLE_TYPE_sphere,
                                .center = vec3_new(0.0, 0.0, -1.0),
                                .radius = 0.5,
                            },
                            (Hittable){
                                .type = HITTABLE_TYPE_sphere,
                                .center = vec3_new(0.0, -100.5, -1.0),
                                .radius = 100,
                            }};
  const size_t world_len = sizeof(world) / sizeof(Hittable);

  Camera camera = camera_new();

  uint8_t *image =
      malloc(sizeof(uint8_t) * image_width * image_height * num_channels);

  for (int i = image_height - 1; i >= 0; i -= 1) {
    fprintf(stderr, "\rScanlines remaining: %d ", i);
    for (int j = 0; j < image_width; j += 1) {
      Color color = vec3_origin();
      for (int s = 0; s < samples_per_pixel; s += 1) {
        double u = ((double)j + random_double()) / (double)(image_width - 1);
        double v = ((double)i + random_double()) / (double)(image_height - 1);
        Ray ray = camera_get_ray(camera, u, v);
        color = vec3_add(color, ray_color(ray, world, world_len, max_depth));
      }
      color = vec3_div_scalar(color, samples_per_pixel);

      uint8_t r, g, b;
      vec3_to_color(color, &r, &g, &b);

      int offset = (image_width * (image_height - i - 1) + j) * num_channels;
      image[offset] = r;
      image[offset + 1] = g;
      image[offset + 2] = b;
    }
  }

  stbi_write_png("out.png", image_width, image_height, num_channels, image,
                 image_width * num_channels);
  free(image);
  fprintf(stderr, "\nDone!\n");

  return 0;
}
