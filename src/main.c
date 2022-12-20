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

#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>

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

void load_file_helper(void *ctx, const char *filename, const int is_mtl,
                      const char *obj_filename, char **buffer, size_t *len);

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

  // test object loading
  tinyobj_shape_t *shape = NULL;
  tinyobj_material_t *material = NULL;
  tinyobj_attrib_t attrib;

  size_t num_shapes;
  size_t num_materials;

  tinyobj_attrib_init(&attrib);

  int result = tinyobj_parse_obj(
      &attrib, &shape, &num_shapes, &material, &num_materials, "tests/cube.obj",
      load_file_helper, NULL, TINYOBJ_FLAG_TRIANGULATE);
  if (result != TINYOBJ_SUCCESS) {
    fprintf(stderr, "filed to load cube.obj\n");
    return 1;
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
  options_free(opts);
  image_free(image);
  world_free(world);
  free(json_src);
  return status;
}

void load_file_helper(void *ctx, const char *filename, const int is_mtl,
                      const char *obj_filename, char **buffer, size_t *len) {
  // supress warnings
  (void)ctx;
  (void)obj_filename;
  (void)is_mtl;

  long string_size = 0, read_size = 0;
  FILE *handler = fopen(filename, "r");

  if (handler) {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char *)malloc(sizeof(char) * (string_size + 1));
    read_size = fread(*buffer, sizeof(char), (size_t)string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size) {
      free(buffer);
      *buffer = NULL;
    }
    fclose(handler);
  }

  *len = read_size;
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
