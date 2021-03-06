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

#ifndef INCLUDED_RAY_LIGHT_H
#define INCLUDED_RAY_LIGHT_H

#include "gsl/gsl_vector.h"

typedef enum RAY_LIGHT_TYPE {
  RAY_LIGHT_TYPE_directional,
  RAY_LIGHT_TYPE_point,
} RAY_LIGHT_TYPE;

typedef struct RayLight {
  RAY_LIGHT_TYPE type;
  union {
    struct { // type = directional
      gsl_vector *direction;
    };
    struct { // type = point
      gsl_vector *position;
    };
  };
  gsl_vector *color;
  double intensity;
} RayLight;

gsl_vector *ray_light_direction_from(const RayLight *light, gsl_vector *hit_point);

double ray_light_intensity(const RayLight *light, gsl_vector *hit_point);

double ray_light_distance(const RayLight *light, gsl_vector *hit_point);

void ray_free_light(RayLight *light);

#endif // ifndef INCLUDED_RAY_LIGHT_H
