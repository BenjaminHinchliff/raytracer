#ifndef WORLD_H
#define WORLD_H

#include "camera.h"
#include <stddef.h>

typedef struct Hittable Hittable;

typedef struct World {
  Camera camera;
  Hittable *objects;
  size_t num_objects;
} World;

#endif
