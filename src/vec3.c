#include "vec3.h"
#include "util.h"

#include <math.h>
#include <stdbool.h>

Vec3 vec3_origin() { return vec3_new(0.0, 0.0, 0.0); }

Vec3 vec3_new(double x, double y, double z) { return (Vec3){x, y, z}; }

void vec3_neg(Vec3 *v) {
  v->x = -v->x;
  v->y = -v->y;
  v->z = -v->z;
}

void vec3_unit_vector(Vec3 *v) { vec3_div_scalar(v, vec3_length(v)); }

void vec3_add(Vec3 *u, const Vec3 *v) {
  u->x += v->x;
  u->y += v->y;
  u->z += v->z;
}

void vec3_sub(Vec3 *u, const Vec3 *v) {
  u->x -= v->x;
  u->y -= v->y;
  u->z -= v->z;
}

void vec3_mul(Vec3 *u, const Vec3 *v) {
  u->x *= v->x;
  u->y *= v->y;
  u->z *= v->z;
}

void vec3_cross(Vec3 *u, const Vec3 *v) {
  Vec3 tmp = {.x = u->y * v->z - v->z * u->y,
              .y = u->z * v->x - u->x * v->z,
              .z = u->x * v->y - u->y * v->x};
  *u = tmp;
}

void vec3_mul_scalar(Vec3 *v, double t) {
  v->x *= t;
  v->y *= t;
  v->z *= t;
}

void vec3_div_scalar(Vec3 *v, double t) { vec3_mul_scalar(v, 1 / t); }

void vec3_lerp(Vec3 *u, const Vec3 *v, double t) {
  vec3_mul_scalar(u, 1.0 - t);
  Vec3 cv = *v;
  vec3_mul_scalar(&cv, t);
  vec3_add(u, &cv);
}

double vec3_dot(const Vec3 *u, const Vec3 *v) {
  return u->x * v->x + u->y * v->y + u->z * v->z;
}

double vec3_length(const Vec3 *v) { return sqrt(vec3_length_squared(v)); }

double vec3_length_squared(const Vec3 *v) {
  return v->x * v->x + v->y * v->y + v->z * v->z;
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
    if (vec3_length_squared(&p) < 1.0) {
      return p;
    }
  }
}

Vec3 vec3_random_unit_sphere() {
  Vec3 v = vec3_random_in_unit_sphere();
  vec3_unit_vector(&v);
  return v;
}
