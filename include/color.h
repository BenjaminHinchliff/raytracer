#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <stdint.h>
#include <cglm/vec4.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef vec4 color;

void vec4_to_color(color color, uint8_t *r, uint8_t *g, uint8_t *b);

#ifdef __cplusplus
}
#endif

#endif
