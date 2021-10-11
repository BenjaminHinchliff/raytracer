#include "color.h"

void vec3_to_color(const vec3 color, uint8_t *r, uint8_t *g, uint8_t *b) {
  *r = (int)(255.999 * color.x);
  *g = (int)(255.999 * color.y);
  *b = (int)(255.999 * color.z);
}
