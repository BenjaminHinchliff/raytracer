#ifndef TRACE_H
#define TRACE_H

#include "color.h"
#include "image.h"
#include "ray.h"
#include "world.h"

void trace_ray(World *world, Ray ray, uint32_t state[4], color color_out);

Image *trace_rows(World *world, uint32_t state[4]);

#endif
