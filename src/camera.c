#include "camera.h"
#include "ray.h"

#include <cglm/vec4.h>

Camera camera_new(double viewport_height, double viewport_width,
                  double focal_length) {
  Camera camera = {
      .origin = GLM_VEC4_ZERO_INIT,
      .horizontal = {viewport_width, 0.0, 0.0, 0.0},
      .vertical = {0.0, viewport_height, 0.0, 0.0},
  };
  glm_vec4_copy((vec4){camera.origin[0] - viewport_width / 2.0,
                       camera.origin[1] - viewport_height / 2.0, -focal_length,
                       0.0},
                camera.lower_left_corner);

  return camera;
}

Ray camera_get_ray(Camera camera, double u, double v) {
  vec4 dir;
  glm_vec4_copy(camera.lower_left_corner, dir);
  glm_vec4_muladds(camera.horizontal, u, dir);
  glm_vec4_muladds(camera.vertical, v, dir);
  glm_vec4_sub(dir, camera.origin, dir);

  return ray_new(camera.origin, dir);
}
