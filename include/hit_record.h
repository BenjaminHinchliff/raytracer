#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "ray.h"

#include <cglm/vec4.h>

#include <stdbool.h>

typedef struct Material Material;

typedef struct HitRecord {
  double t;
  vec4 p;
  vec4 normal;
  bool front_face;
  const Material *material;
} HitRecord;

void hit_record_set_face_normal(HitRecord *rec, Ray ray);

#endif
