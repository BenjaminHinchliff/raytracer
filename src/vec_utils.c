//  a small and simple raytracer
//  Copyright (C) 2021  Benjamin Hinchliff
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//  USA

#include "ray/vec_utils.h"

#include <assert.h>

#include "gsl/gsl_blas.h"

gsl_vector *ray_create_vec3(double x, double y, double z) {
  gsl_vector *vec = gsl_vector_alloc(3);
  gsl_vector_set(vec, 0, x);
  gsl_vector_set(vec, 1, y);
  gsl_vector_set(vec, 2, z);
  return vec;
}

// I couldn't find a utility function for this in blas
void ray_vec_normalize(gsl_vector *vec) {
  double vec_len = gsl_blas_dnrm2(vec);
  gsl_vector_scale(vec, 1.0 / vec_len);
}

void ray_vec_clamp(gsl_vector *vec) {
  for (size_t i = 0; i < vec->size; ++i) {
    double comp = gsl_vector_get(vec, i);
    comp = comp < 0.0 ? 0.0 : comp;
    comp = comp > 1.0 ? 1.0 : comp;
    gsl_vector_set(vec, i, comp);
  }
}

void ray_vec3_cross(gsl_vector *u, gsl_vector *v) {
  assert(u->size == 3 && "u vector must be of size 3");
  assert(v->size == 3 && "v vector must be of size 3");
  double ux = gsl_vector_get(u, 0);
  double uy = gsl_vector_get(u, 1);
  double uz = gsl_vector_get(u, 2);
  double vx = gsl_vector_get(v, 0);
  double vy = gsl_vector_get(v, 1);
  double vz = gsl_vector_get(v, 2);

  double px = uy * vz - uz * vy;
  double py = uz * vx - ux * vz;
  double pz = ux * vy - uy * vx;
  gsl_vector_set(u, 0, px);
  gsl_vector_set(u, 1, py);
  gsl_vector_set(u, 2, pz);
}
