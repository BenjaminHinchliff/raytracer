#include "vec3.h"

#include <math.h>

Vec3 vec3_origin() { return vec3_new(0.0, 0.0, 0.0); }

Vec3 vec3_new(double x, double y, double z) { return (Vec3){x, y, z}; }

Vec3 vec3_neg(Vec3 v) { return (Vec3){-v.x, -v.y, -v.z}; }

Vec3 vec3_unit_vector(Vec3 v) { return vec3_div_scalar(v, vec3_length(v)); }

double vec3_length(Vec3 v) { return sqrt(vec3_length_squared(v)); }

double vec3_length_squared(Vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

Vec3 vec3_add(Vec3 u, Vec3 v) {
  return (Vec3){u.x + v.x, u.y + v.y, u.z + v.z};
}

Vec3 vec3_sub(Vec3 u, Vec3 v) {
  return (Vec3){u.x - v.x, u.y - v.y, u.z - v.z};
}

Vec3 vec3_mul(Vec3 u, Vec3 v) {
  return (Vec3){u.x * v.x, u.y * v.y, u.z * v.z};
}

double vec3_dot(Vec3 u, Vec3 v) { return u.x * v.x + u.y * v.y + u.z * v.z; }

Vec3 vec3_cross(Vec3 u, Vec3 v) {
  return (Vec3){u.y * v.z - v.z * u.y, u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
}

Vec3 vec3_mul_scalar(Vec3 v, double t) {
  return (Vec3){v.x * t, v.y * t, v.z * t};
}

Vec3 vec3_div_scalar(Vec3 v, double t) { return vec3_mul_scalar(v, 1 / t); }
