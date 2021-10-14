#include "camera.h"
#include "ray.h"
#include "vec3.h"

Point3 get_lower_left_corner(const Camera camera, double focal_length) {
  Point3 llc = camera.origin;
  Vec3 half_horiz = camera.horizontal;
  vec3_div_scalar(&half_horiz, 2.0);
  vec3_sub(&llc, &half_horiz);
  Vec3 half_vert = camera.vertical;
  vec3_div_scalar(&half_vert, 2.0);
  vec3_sub(&llc, &half_vert);
  Vec3 focal_vec = vec3_new(0.0, 0.0, focal_length);
  vec3_sub(&llc, &focal_vec);
  return llc;
}

Camera camera_new() {
  const double aspect_ratio = 16.0 / 9.0;
  const double viewport_height = 2.0;
  const double viewport_width = aspect_ratio * viewport_height;
  const double focal_length = 1.0;

  Camera camera = {
      .origin = vec3_origin(),
      .horizontal = vec3_new(viewport_width, 0.0, 0.0),
      .vertical = vec3_new(0.0, viewport_height, 0.0),
  };
  camera.lower_left_corner = get_lower_left_corner(camera, focal_length);

  return camera;
}

Ray camera_get_ray(const Camera *camera, double u, double v) {
  Vec3 dir = camera->lower_left_corner;

  Vec3 scaled_u = camera->horizontal;
  vec3_mul_scalar(&scaled_u, u);
  vec3_add(&dir, &scaled_u);

  Vec3 scaled_v = camera->vertical;
  vec3_mul_scalar(&scaled_v, v);
  vec3_add(&dir, &scaled_v);

  vec3_sub(&dir, &camera->origin);

  return ray_new(camera->origin, dir);
}
