#include "util.h"

double degrees_to_radians(double degrees) { return degrees * M_PI / 180.0; }

double random_double() { return rand() / (RAND_MAX + 1.0); }

double random_double_range(double min, double max) {
  return min + (max - min) * random_double();
}

double clamp(double v, double min, double max) {
  return (v < min) ? min : (v > max) ? max : v;
}
