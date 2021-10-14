#include "color.h"
#include "util.h"

#include <math.h>

void vec3_to_color(const Vec3 *color, uint8_t *r, uint8_t *g, uint8_t *b) {
  double gr = sqrt(color->x);
  double gg = sqrt(color->y);
  double gb = sqrt(color->z);

  *r = (int)(255.999 * clamp(gr, 0.0, 1.0));
  *g = (int)(255.999 * clamp(gg, 0.0, 1.0));
  *b = (int)(255.999 * clamp(gb, 0.0, 1.0));
}
