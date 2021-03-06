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

#ifndef INCLUDED_RAY_SCENE_H
#define INCLUDED_RAY_SCENE_H

#include "light.h"
#include "objects.h"

typedef struct RayScene {
  int width;
  int height;
  double fov;
  double shadow_bias;
  double max_recursion_depth;
  gsl_vector *background;
  int num_objects;
  RayObject *objects;
  int num_lights;
  RayLight *lights;
} RayScene;

void ray_free_scene(RayScene *scene);

#endif // ifndef INCLUDED_RAY_SCENE_H
