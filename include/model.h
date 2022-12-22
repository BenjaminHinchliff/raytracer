#ifndef MODEL_H
#define MODEL_H

#include "triangle.h"

typedef struct Model {
  Triangle *tris;
  size_t num_tris;
} Model;

bool model_load(const char *path, Model *model);

bool model_hit(Model model, Ray ray, double t_min, double t_max,
               const Material *material, HitRecord *rec);

void model_free(Model *model);

#endif
