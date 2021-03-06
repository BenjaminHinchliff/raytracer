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

#ifndef INCLUDED_RAY_OBJECTS_H
#define INCLUDED_RAY_OBJECTS_H

#include <stdbool.h>

#include "gsl/gsl_vector.h"

#include "material.h"
#include "tex_coord.h"

enum RAY_OBJECT_TYPE {
  RAY_OBJECT_TYPE_sphere,
  RAY_OBJECT_TYPE_plane,
};

typedef struct RayObject {
  enum RAY_OBJECT_TYPE type;
  union {
    struct { // type = sphere
      gsl_vector *center;
      double radius;
    };
    struct { // type = plane
      gsl_vector *point;
      gsl_vector *normal;
    };
  };
  RayMaterial material;
} RayObject;

RayTexCoord ray_object_tex_coord(const RayObject *object,
                                 gsl_vector *hit_point);

void ray_free_object(RayObject *sphere);

#endif // ifndef INCLUDED_RAY_OBJECTS_H
