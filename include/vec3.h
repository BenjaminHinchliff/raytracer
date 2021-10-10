#ifndef VEC3_H
#define VEC3_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vec3 {
  double x;
  double y;
  double z;
} vec3;

vec3 vec3_origin();
vec3 vec3_new(double x, double y, double z);

// TODO: add mutator methods for performance?
vec3 vec3_neg(vec3 v);
vec3 vec3_length(vec3 v);
vec3 vec3_length_squared(vec3 v);
vec3 vecr_unit_vector(vec3 v);

vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, vec3 b);
vec3 vec3_div(vec3 a, vec3 b);
vec3 vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);

vec3 vec3_mul_scalar(vec3 v, double t);
vec3 vec3_div_scalar(vec3 v, double t);

#ifdef __cplusplus
}
#endif

#endif
