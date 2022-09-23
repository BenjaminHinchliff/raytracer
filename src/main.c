#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "png_write.h"
#include "pngconf.h"
#include "ray.h"
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

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 480;
const int image_height = (int)(image_width / aspect_ratio);
const int samples_per_pixel = 128;
const int max_depth = 100;

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass);
double time_as_double(void);

int main(void) {
  srand(time(NULL));

  Camera camera = camera_new(aspect_ratio);

  // const Material mat_left = {
  //     .type = MATERIAL_TYPE_dielectric,
  //     .ir = 1.5,
  // };
  Hittable objects[] = {
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {-1.0, 0.0, -1.0, 0.0},
      //     .radius = 0.5,
      //     .material = mat_left,
      // },
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {-1.0, 0.0, -1.0, 0.0},
      //     .radius = -0.4,
      //     .material = mat_left,
      // },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, 0.0, -1.0, 0.0},
          .radius = 0.5,
          .material =
              {
                  .type = MATERIAL_TYPE_lambertian,
                  .l_albedo = {0.1, 0.2, 0.5, 1.0},
              },
      },
      // {
      //     .type = HITTABLE_TYPE_sphere,
      //     .center = {1.0, 0.0, -1.0, 0.0},
      //     .radius = 0.5,
      //     .material =
      //         {
      //             .type = MATERIAL_TYPE_metal,
      //             .m_albedo = {0.8, 0.6, 0.2, 1.0},
      //             .fuzz = 0.3,
      //         },
      // },
      {
          .type = HITTABLE_TYPE_sphere,
          .center = {0.0, -100.5, -1.0, 0.0},
          .radius = 100,
          .material =
              {
                  .type = MATERIAL_TYPE_lambertian,
                  .l_albedo = {0.6, 0.8, 0.0, 1.0},
              },
      },
  };
  size_t num_objects = sizeof(objects) / sizeof(Hittable);
  World world = {
      .camera = camera,
      .objects = objects,
      .num_objects = num_objects,
  };

  Image *image = image_new(image_width, image_height);

  // uint32_t rng_state = rand();

  WorkQueue *work_queue = work_queue_new(image->height);
  for (int row = 0; row < image->height; row++) {
    WorkOrder order = (WorkOrder){
        .image = image,
        .world = &world,
        .start_row_index = row,
        .end_row_index = row + 1,
        .samples = samples_per_pixel,
        .max_depth = max_depth,
    };

    work_queue_add(work_queue, order);
  }

  double start_time = time_as_double();

  work_queue_run(work_queue);

  double end_time = time_as_double();
  fprintf(stderr, "\n");

  image_write_png(image, "traced.png", write_row_progress_callback);

  fprintf(stderr, "\nDone!\n");

  double delta_time = end_time - start_time;
  fprintf(stderr, "Took %f seconds at %f ns/ray\n", delta_time,
          (delta_time * 1e+9) /
              (image->width * image->height * samples_per_pixel));

  work_queue_free(work_queue);
  image_free(image);
  return 0;
}

void write_row_progress_callback(png_structp png_ptr, png_uint_32 row,
                                 int pass) {
  (void)png_ptr, (void)pass;
  // avoid extra unneeded printing
  if (row % 32 == 0) {
    fprintf(stderr, "\rPng write rows remaining: %d ", image_height - row);
  }
}

double time_as_double(void) {
#ifdef _POSIX_TIMERS
  struct timespec now;
  if (clock_gettime(CLOCK_REALTIME, &now) != 0) {
    fprintf(stderr, "failed to get time!");
    return 0;
  }
  return now.tv_sec + now.tv_nsec * 1e-9;
#else
  return (double)time(NULL);
#endif
}
