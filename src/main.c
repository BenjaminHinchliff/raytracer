#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "options.h"
#include "png_write.h"
#include "pngconf.h"
#include "ray.h"
#include "trace.h"
#include "util.h"
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

char *read_file_to_string(const char *path);

int main(int argc, char *argv[]) {
  int status = 0;
  char *json_src = NULL;
  World *world = NULL;
  Image *image = NULL;
  Options *opts = NULL;

  srand(time(NULL));

  opts = options_parse(argc, argv);
  if (opts == NULL) {
    status = 1;
    goto end;
  }

  // get source from file
  json_src = read_file_to_string(opts->world_path);
  if (json_src == NULL) {
    fprintf(stderr, "failed to open scene file\n");
    status = 1;
    goto end;
  }

  fprintf(stderr, "Loading world file...\n");

  bool success = world_load(json_src, &world);
  if (!success) {
    fprintf(stderr, "failed to load world\n");
    status = 1;
    goto end;
  }

  fprintf(stderr, "Raytracing...\n");

  uint32_t rng_state[4] = {rand(), rand(), rand(), 0.0};

  double start_time = time_as_double();

  image = trace_rows(world, rng_state);

  double end_time = time_as_double();

  fprintf(stderr, "Writing out png...\n");

  image_write_png(image, opts->output_path, NULL);

  fprintf(stderr, "Done!\n");

  double delta_time = end_time - start_time;
  fprintf(stderr, "Took %f seconds at %f ns/ray\n", delta_time,
          (delta_time * 1e+9) / (world->screen.width * world->screen.height *
                                 world->screen.samples));

end:
  image_free(image);
  world_free(world);
  free(json_src);
  return status;
}

char *read_file_to_string(const char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    return NULL;
  }

  // slow and lazy but it works
  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = malloc(length + 1);
  if (str == NULL) {
    return NULL;
  }

  fread(str, 1, length, f);

  str[length] = '\0';

  fclose(f);

  return str;
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
