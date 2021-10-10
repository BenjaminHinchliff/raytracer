#include "vec3.h"

#include <math.h>

vec3 vec3_origin() { return vec3_new(0.0, 0.0, 0.0); }

vec3 vec3_new(double x, double y, double z) { return (vec3){x, y, z}; }

vec3 vec3_neg(vec3 v) { return (vec3){-v.x, -v.y, -v.z}; }

vec3 vec3_unit_vector(vec3 v) { return vec3_div_scalar(v, vec3_length(v)); }

double vec3_length(vec3 v) { return sqrt(vec3_length_squared(v)); }

double vec3_length_squared(vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

vec3 vec3_add(vec3 u, vec3 v) {
  return (vec3){u.x + v.x, u.y + v.y, u.z + v.z};
}

vec3 vec3_sub(vec3 u, vec3 v) {
  return (vec3){u.x - v.x, u.y - v.y, u.z - v.z};
}

vec3 vec3_mul(vec3 u, vec3 v) {
  return (vec3){u.x * v.x, u.y * v.y, u.z * v.z};
}

double vec3_dot(vec3 u, vec3 v) { return u.x * v.x + u.y * v.y + u.z * v.z; }

vec3 vec3_cross(vec3 u, vec3 v) {
  return (vec3){u.y * v.z - v.z * u.y, u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
}

vec3 vec3_mul_scalar(vec3 v, double t) {
  return (vec3){v.x * t, v.y * t, v.z * t};
}

vec3 vec3_div_scalar(vec3 v, double t) { return vec3_mul_scalar(v, 1 / t); }
