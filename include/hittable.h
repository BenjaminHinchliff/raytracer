#ifndef HITTABLE_H
#define HITTABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "ray.h"
#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HitRecord {
  Point3 p;
  Vec3 normal;
  double t;
  bool front_face;
} HitRecord;

enum HITTABLE_TYPE {
  HITTABLE_TYPE_sphere,
};
#define NUM_HITTABLE_TYPES (1)

typedef struct Hittable {
  enum HITTABLE_TYPE type;
  union {
    struct { // type = sphere
      Point3 center;
      double radius;
    };
  };
} Hittable;

bool hittable_hit(const Hittable sphere, const Ray ray, double t_min,
                  double t_max, HitRecord *rec);

bool hittable_hit_multiple(const Hittable *hittables, size_t num, const Ray ray,
                           double t_min, double t_max, HitRecord *rec);

#ifdef __cplusplus
}
#endif

#endif
