#include "hittable.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "color.h"
#include "ray.h"
#include "vec3.h"

Color ray_color(const Ray ray, const Hittable *world, const size_t world_len) {
  HitRecord rec;
  bool hit = hittable_hit_multiple(world, world_len, ray, 0, INFINITY, &rec);
  if (hit) {
    const Vec3 N = rec.normal;
    return vec3_mul_scalar(vec3_new(N.x + 1.0, N.y + 1.0, N.z + 1.0), 0.5);
  }
  Vec3 unit_dir = vec3_unit_vector(ray.dir);
  double t = 0.5 * (unit_dir.y + 1.0);
  return vec3_lerp(vec3_new(1.0, 1.0, 1.0), vec3_new(0.5, 0.7, 1.0), t);
}

Point3 get_lower_left_corner(Point3 origin, Vec3 horizontal, Vec3 vertical,
                             double focal_length) {
  Point3 llc = origin;
  llc = vec3_sub(llc, vec3_div_scalar(horizontal, 2.0));
  llc = vec3_sub(llc, vec3_div_scalar(vertical, 2.0));
  llc = vec3_sub(llc, vec3_new(0.0, 0.0, focal_length));
  return llc;
}

int main(int argc, char **argv) {
  const double aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = (int)(image_width / aspect_ratio);
  const int num_channels = 3;

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

  double viewport_height = 2.0;
  double viewport_width = aspect_ratio * viewport_height;
  double focal_length = 1.0;

  Point3 origin = vec3_new(0, 0, 0);
  Vec3 horizontal = vec3_new(viewport_width, 0, 0);
  Vec3 vertical = vec3_new(0, viewport_height, 0);
  Point3 lower_left_corner =
      get_lower_left_corner(origin, horizontal, vertical, focal_length);

  uint8_t *image =
      malloc(sizeof(uint8_t) * image_width * image_height * num_channels);

  for (int i = image_height - 1; i >= 0; i -= 1) {
    fprintf(stderr, "\rScanlines remaining: %d ", i);
    for (int j = 0; j < image_width; j += 1) {
      double u = (double)j / (double)(image_width - 1);
      double v = (double)i / (double)(image_height - 1);
      Vec3 dir = lower_left_corner;
      dir = vec3_add(dir, vec3_mul_scalar(horizontal, u));
      dir = vec3_add(dir, vec3_mul_scalar(vertical, v));
      dir = vec3_sub(dir, origin);
      Ray ray = ray_new(origin, dir);

      Color color = ray_color(ray, world, world_len);

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
