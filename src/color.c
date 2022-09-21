#include "color.h"
#include "util.h"

#include <math.h>

void vec4_to_color(color color, uint8_t *r, uint8_t *g, uint8_t *b) {
  double gr = sqrt(color[0]);
  double gg = sqrt(color[1]);
  double gb = sqrt(color[2]);

  *r = (int)(255.999 * clamp(gr, 0.0, 1.0));
  *g = (int)(255.999 * clamp(gg, 0.0, 1.0));
  *b = (int)(255.999 * clamp(gb, 0.0, 1.0));
}
