#include "hittable_list.h"

#include <stdlib.h>

bool hittable_list_new(size_t num_hittables, HittableList *list) {
  Hittable *hittables = malloc(sizeof(*hittables) * num_hittables);
  if (hittables == NULL) {
    return false;
  }

  *list = (HittableList){
      .hittables = hittables,
      .num_hittables = num_hittables,
  };

  return true;
}

bool hittable_list_hit(const HittableList *list, Ray ray, double t_min,
                       double t_max, HitRecord *rec) {
  HitRecord tmp_rec;
  bool hit = false;
  double closest = t_max;

  for (size_t i = 0; i < list->num_hittables; i++) {
    if (hittable_hit(list->hittables[i], ray, t_min, closest, &tmp_rec)) {
      hit = true;
      closest = tmp_rec.t;
      *rec = tmp_rec;
    }
  }

  return hit;
}

void hittable_list_free(HittableList list) { free(list.hittables); }
