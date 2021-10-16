#ifndef VEC3_H
#define VEC3_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Vec3 {
  double x;
  double y;
  double z;
} Vec3;

typedef Vec3 Point3;

Vec3 vec3_origin();
Vec3 vec3_new(double x, double y, double z);

// Mutator methods
void vec3_neg(Vec3 *v);
void vec3_unit_vector(Vec3 *v);

void vec3_add(Vec3 *u, const Vec3 *v);
void vec3_sub(Vec3 *u, const Vec3 *v);
void vec3_mul(Vec3 *u, const Vec3 *v);
void vec3_cross(Vec3 *u, const Vec3 *v);

void vec3_mul_scalar(Vec3 *v, double t);
void vec3_div_scalar(Vec3 *v, double t);

void vec3_lerp(Vec3 *u, const Vec3 *v, double t);
void vec3_reflect(Vec3 *v, const Vec3 *n);
void vec3_refract(Vec3 *uv, const Vec3 *n, double etai_over_etat);

double vec3_dot(const Vec3 *u, const Vec3 *v);
double vec3_length(const Vec3 *v);
double vec3_length_squared(const Vec3 *v);

Vec3 vec3_random();
Vec3 vec3_random_range(double min, double max);
Vec3 vec3_random_in_unit_sphere();
Vec3 vec3_random_unit_sphere();

bool vec3_near_zero(const Vec3 *v);

#ifdef __cplusplus
}
#endif

#endif
