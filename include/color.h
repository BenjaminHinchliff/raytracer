#ifndef COLOR_H
#define COLOR_H

#include <cglm/vec4.h>
#include <stdint.h>

typedef vec4 color;

void vec4_to_color(color color, uint8_t *r, uint8_t *g, uint8_t *b);

#endif
