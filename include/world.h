#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"

typedef struct Screen {
  unsigned int width;
  unsigned int height;
  unsigned int samples;
  unsigned int max_depth;
} Screen;

typedef struct World {
  Screen screen;
  Camera camera;
  HittableList objects;
  // here to keep track of the pointer
  Material *materials;
} World;

bool world_load(const char *json_src, World **world);

void world_free(World *world);

#endif
