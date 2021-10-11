#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef Vec3 Color;

void vec3_to_color(const Color color, uint8_t *r, uint8_t *g, uint8_t *b);

#ifdef __cplusplus
}
#endif

#endif
