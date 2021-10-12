#include "camera.h"
#include "vec3.h"

Point3 get_lower_left_corner(const Camera camera, double focal_length) {
  Point3 llc = camera.origin;
  llc = vec3_sub(llc, vec3_div_scalar(camera.horizontal, 2.0));
  llc = vec3_sub(llc, vec3_div_scalar(camera.vertical, 2.0));
  llc = vec3_sub(llc, vec3_new(0.0, 0.0, focal_length));
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
