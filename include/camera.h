#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Camera {
  Point3 origin;
  Point3 lower_left_corner;
  Vec3 horizontal;
  Vec3 vertical;
} Camera;

Camera camera_new();

Ray camera_get_ray(const Camera camera, double u, double v);

#ifdef __cplusplus
}
#endif

#endif
