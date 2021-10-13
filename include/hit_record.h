#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include <stdbool.h>

#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Material Material;

typedef struct HitRecord {
  Point3 p;
  Vec3 normal;
  const Material *material;
  double t;
  bool front_face;
} HitRecord;

#ifdef __cplusplus
}
#endif

#endif
