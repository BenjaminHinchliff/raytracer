#ifndef TRACE_H
#define TRACE_H

#include "color.h"
#include "image.h"
#include "ray.h"
#include "world.h"

void trace_ray(World *world, Ray ray, int depth, uint32_t state[4],
               color color_out);

void trace_rows(World *world, uint32_t state[4], int start, int end,
                int samples, int max_depth, Image *image);

#endif
