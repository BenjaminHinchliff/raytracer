#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "png_write.h"
#include "pngconf.h"
#include "ray.h"
#include "trace.h"
#include "util.h"
#include "work_queue.h"
#include "world.h"

#include <cglm/vec4.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32) ||                \
    defined(__CYGWIN__)
#include <unistd.h>
#endif

const double viewport_height = 2.0;
const double focal_length = 1.0;

double time_as_double(void);

int main(void) {
  srand(time(NULL));

  Screen screen = {
      .width = 720,
      .height = 480,
      .samples = 64,
  };

  const double aspect_ratio = (double)screen.width / (double)screen.height;

  Camera camera = camera_new(viewport_height, aspect_ratio, focal_length);

  const Material mat_left = {
      .type = MATERIAL_TYPE_dielectric,
      .ir = 1.5,
  };
  const Material mat_center = {
      .type = MATERIAL_TYPE_lambertian,
      .l_albedo = {0.1, 0.2, 0.5, 1.0},
  };
  const Material mat_right = {
      .type = MATERIAL_TYPE_metal,
      .m_albedo = {0.8, 0.6, 0.2, 1.0},
      .fuzz = 0.3,
  };
  const Material mat_floor = {
      .type = MATERIAL_TYPE_lambertian,
      .l_albedo = {0.6, 0.8, 0.0, 1.0},
  };
  Hittable objects[] = {
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {-1.0, 0.0, -1.0, 0.0},
          .radius = 0.5,
          .material = &mat_left,
      },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {-1.0, 0.0, -1.0, 0.0},
          .radius = -0.4,
          .material = &mat_left,
      },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, 0.0, -1.0, 0.0},
          .radius = 0.5,
          .material = &mat_center,
      },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {1.0, 0.0, -1.0, 0.0},
          .radius = 0.5,
          .material = &mat_right,
      },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, -100.5, -1.0, 0.0},
          .radius = 100,
          .material = &mat_floor,
      },
  };
  size_t num_objects = sizeof(objects) / sizeof(Hittable);

  World world = {
      .screen = screen,
      .camera = camera,
      .objects = objects,
      .num_objects = num_objects,
      .max_depth = 32,
  };

  double start_time = time_as_double();

  fprintf(stderr, "Raytracing...\n");

  uint32_t rng_state[4] = {rand(), rand(), rand(), 0.0};

  Image *image = trace_rows(&world, rng_state);

  double end_time = time_as_double();

  fprintf(stderr, "Writing out png...\n");

  image_write_png(image, "traced.png", NULL);

  fprintf(stderr, "Done!\n");

  double delta_time = end_time - start_time;
  fprintf(stderr, "Took %f seconds at %f ns/ray\n", delta_time,
          (delta_time * 1e+9) / (world.screen.width * world.screen.height *
                                 world.screen.samples));

  image_free(image);
  return 0;
}

double time_as_double(void) {
#ifdef _POSIX_TIMERS
  struct timespec now;
  if (clock_gettime(CLOCK_REALTIME, &now) != 0) {
    fprintf(stderr, "failed to get time!");
    return 1;
  }
  return now.tv_sec + now.tv_nsec * 1e-9;
#else
  return (double)time(NULL);
#endif
}
