#include "hit_record.h"

void hit_record_set_face_normal(HitRecord *rec, Ray ray) {
  rec->front_face = glm_vec4_dot(ray.dir, rec->normal) < 0.0;
  if (!rec->front_face) {
    glm_vec4_negate(rec->normal);
  }
}
