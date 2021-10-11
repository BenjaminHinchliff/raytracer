#ifndef VEC3_H
#define VEC3_H

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

// TODO: add mutator methods for performance?
Vec3 vec3_neg(Vec3 v);
Vec3 vec3_unit_vector(Vec3 v);
double vec3_length(Vec3 v);
double vec3_length_squared(Vec3 v);

Vec3 vec3_add(Vec3 u, Vec3 v);
Vec3 vec3_sub(Vec3 u, Vec3 v);
Vec3 vec3_mul(Vec3 u, Vec3 v);
double vec3_dot(Vec3 u, Vec3 v);
Vec3 vec3_cross(Vec3 u, Vec3 v);

Vec3 vec3_mul_scalar(Vec3 v, double t);
Vec3 vec3_div_scalar(Vec3 v, double t);

Vec3 vec3_lerp(Vec3 u, Vec3 v, double t);

#ifdef __cplusplus
}
#endif

#endif
