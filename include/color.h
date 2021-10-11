#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void vec3_to_color(const vec3 color, uint8_t *r, uint8_t *g, uint8_t *b);

#ifdef __cplusplus
}
#endif

#endif
