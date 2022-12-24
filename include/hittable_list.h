#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

typedef struct HittableList {
  Hittable *hittables;
  size_t num_hittables;
} HittableList;

bool hittable_list_new(size_t num_hittables, HittableList *list);

bool hittable_list_hit(const HittableList list, Ray ray, double t_min,
                       double t_max, HitRecord *rec);

void hittable_list_free(HittableList list);

#endif
