#ifndef WORLD_H
#define WORLD_H

#include <stddef.h>

#include "camera.h"
#include "hittable.h"

typedef struct Screen {
  unsigned int width;
  unsigned int height;
  unsigned int samples;
} Screen;

typedef struct World {
  Screen screen;
  Camera camera;
  Hittable *objects;
  size_t num_objects;
  unsigned int max_depth;
} World;

// World load_world()

#endif
