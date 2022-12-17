#include "camera.h"
#include "ray.h"

#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/vec4.h>
#include <stdio.h>

double to_rad(double deg) { return deg / 180.0 * GLM_PI; }

Camera camera_new(vec4 eye, vec4 center, vec4 up, double viewport_height,
                  double viewport_width, double vfov) {
  float *origin = eye;
  double theta = to_rad(vfov);
  double h = tan(theta / 2.0);
  double aspect = viewport_width / viewport_height;
  viewport_height *= h;
  viewport_width = viewport_height * aspect;
  vec4 horizontal = {viewport_width, 0.0, 0.0, 0.0};
  vec4 vertical = {0.0, viewport_height, 0.0, 0.0};

  mat4 view;
  glm_lookat(origin, center, up, view);
  // lookat function is designed to move objects with a static camera, but here
  // we want to move the camera with staic objects, hence the matrix must be
  // inverted (or I could do it manually but this is easier)
  glm_mat4_inv(view, view);

  glm_mat4_mulv(view, horizontal, horizontal);
  glm_mat4_mulv(view, vertical, vertical);

  vec4 frac_h_2;
  glm_vec4_divs(horizontal, 2.0, frac_h_2);
  vec4 frac_v_2;
  glm_vec4_divs(vertical, 2.0, frac_v_2);

  vec4 w;
  glm_vec4_sub(center, origin, w);
  glm_vec4_normalize(w);

  vec4 llc;
  glm_vec4_copy(origin, llc);
  glm_vec4_sub(llc, frac_h_2, llc);
  glm_vec4_sub(llc, frac_v_2, llc);
  glm_vec4_add(llc, w, llc);

  Camera c;
  glm_vec4_copy(origin, c.origin);
  glm_vec4_copy(horizontal, c.horizontal);
  glm_vec4_copy(vertical, c.vertical);
  glm_vec4_copy(llc, c.lower_left_corner);

  return c;
}

Ray camera_get_ray(Camera camera, double u, double v) {
  vec4 dir;
  glm_vec4_copy(camera.lower_left_corner, dir);
  glm_vec4_muladds(camera.horizontal, u, dir);
  glm_vec4_muladds(camera.vertical, v, dir);
  glm_vec4_sub(dir, camera.origin, dir);

  return ray_new(camera.origin, dir);
}
