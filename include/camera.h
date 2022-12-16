#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

#include <cglm/vec3.h>

typedef struct Camera {
  vec4 origin;
  vec4 lower_left_corner;
  vec4 horizontal;
  vec4 vertical;
} Camera;

Camera camera_new(double viewport_height, double aspect_ratio,
                  double focal_length);

Ray camera_get_ray(Camera camera, double u, double v);

#endif
