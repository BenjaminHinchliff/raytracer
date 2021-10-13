#include "vec3.h"
#include "util.h"

#include <math.h>
#include <stdbool.h>

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

Vec3 vec3_lerp(Vec3 u, Vec3 v, double t) {
  const Vec3 su = vec3_mul_scalar(u, 1.0 - t);
  const Vec3 sv = vec3_mul_scalar(v, t);
  return vec3_add(su, sv);
}

Vec3 vec3_random() {
  return vec3_new(random_double(), random_double(), random_double());
}

Vec3 vec3_random_range(double min, double max) {
  return vec3_new(random_double_range(min, max), random_double_range(min, max),
                  random_double_range(min, max));
}

Vec3 vec3_random_in_unit_sphere() {
  while (true) {
    Vec3 p = vec3_random_range(-1.0, 1.0);
    if (vec3_length_squared(p) < 1.0) {
      return p;
    }
  }
}
