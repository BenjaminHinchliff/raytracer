#include "world.h"
#include "camera.h"
#include "hittable.h"

#include <cjson/cJSON.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// utiltiy macro to make exiting on invalid datatypes easier
// NOTE: does no cleanup on exit so can't be used in main loading code
#define CJSON_ASSERT(validator, value, type)                                   \
  if (!validator(value)) {                                                     \
    fprintf(stderr, #value " must exist and be an " type "\n");                \
    return false;                                                              \
  }

#define CJSON_ASSERT_OBJECT(value) CJSON_ASSERT(cJSON_IsObject, value, "object")
#define CJSON_ASSERT_NUMBER(value) CJSON_ASSERT(cJSON_IsNumber, value, "number")
#define CJSON_ASSERT_ARRAY(value) CJSON_ASSERT(cJSON_IsArray, value, "array")
#define CJSON_ASSERT_STRING(value) CJSON_ASSERT(cJSON_IsString, value, "string")

bool screen_load(cJSON *json_scr, Screen *screen) {
  cJSON *width = cJSON_GetObjectItemCaseSensitive(json_scr, "width");
  CJSON_ASSERT_NUMBER(width);
  screen->width = width->valueint;

  cJSON *height = cJSON_GetObjectItemCaseSensitive(json_scr, "height");
  CJSON_ASSERT_NUMBER(width);
  screen->height = height->valueint;

  cJSON *samples = cJSON_GetObjectItemCaseSensitive(json_scr, "samples");
  CJSON_ASSERT_NUMBER(samples);
  screen->samples = samples->valueint;

  cJSON *max_depth = cJSON_GetObjectItemCaseSensitive(json_scr, "max-depth");
  CJSON_ASSERT_NUMBER(samples);
  screen->max_depth = max_depth->valueint;

  return true;
}

bool camera_load(cJSON *json_cam, const Screen *screen, Camera *camera) {
  cJSON *height = cJSON_GetObjectItemCaseSensitive(json_cam, "height");
  CJSON_ASSERT_NUMBER(height);

  cJSON *focal_length =
      cJSON_GetObjectItemCaseSensitive(json_cam, "focal-length");
  CJSON_ASSERT_NUMBER(focal_length);

  cJSON *width = cJSON_GetObjectItemCaseSensitive(json_cam, "width");
  if (cJSON_IsString(width) && strcmp(width->valuestring, "auto") == 0) {
    double aspect = (double)screen->width / (double)screen->height;
    double width = height->valuedouble * aspect;
    *camera = camera_new(height->valuedouble, width, focal_length->valuedouble);
  } else if (cJSON_IsNumber(width)) {
    *camera = camera_new(height->valuedouble, width->valuedouble,
                         focal_length->valuedouble);
  } else {
    fprintf(stderr, "camera width must be a number or \"auto\"\n");
    return false;
  }

  return true;
}

bool vec4_load(cJSON *json_v, vec4 v) {
  size_t i = 0;
  cJSON *e;
  cJSON_ArrayForEach(e, json_v) {
    if (i >= 4 || !cJSON_IsNumber(e)) {
      return false;
    }

    v[i++] = e->valuedouble;
  }

  // only sucessful if precisely 4 elements were read in
  return i == 4;
}

bool sphere_load(cJSON *json_sph, Hittable *sphere) {
  sphere->type = HITTABLE_TYPE_sphere;

  cJSON *j_center = cJSON_GetObjectItemCaseSensitive(json_sph, "center");
  CJSON_ASSERT_ARRAY(j_center);

  vec4 center;
  if (!vec4_load(j_center, center)) {
    return false;
  }
  glm_vec4_copy(center, sphere->center);

  cJSON *radius = cJSON_GetObjectItemCaseSensitive(json_sph, "radius");
  CJSON_ASSERT_NUMBER(radius);
  sphere->radius = radius->valuedouble;

  cJSON *material = cJSON_GetObjectItemCaseSensitive(json_sph, "material");
  CJSON_ASSERT_STRING(material);

  // TODO: material lookup logic
  sphere->material = NULL;

  return true;
}

bool objects_load(cJSON *json_objs, Hittable *objects) {
  size_t i = 0;
  cJSON *object;
  cJSON_ArrayForEach(object, json_objs) {
    cJSON *sphere = cJSON_GetObjectItemCaseSensitive(object, "sphere");
    if (cJSON_IsObject(sphere)) {
      if (!sphere_load(sphere, &objects[i++])) {
        return false;
      }
    } else {
      fprintf(stderr, "unknown object type");
      return false;
    }
  }

  return true;
}

bool world_load(const char *json_src, World *world) {
  bool success = true;

  // parse world to json object
  cJSON *json = cJSON_Parse(json_src);
  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    success = false;
    goto end;
  }

  char *json_str = cJSON_Print(json);
  printf("%s\n\n", json_str);
  free(json_str);

  // screen
  cJSON *screen = cJSON_GetObjectItemCaseSensitive(json, "screen");
  if (!cJSON_IsObject(screen)) {
    fprintf(stderr, "screen must exist and be an object\n");
    success = false;
    goto end;
  }

  if (!(success = screen_load(screen, &world->screen))) {
    goto end;
  }

  // camera
  cJSON *camera = cJSON_GetObjectItemCaseSensitive(json, "camera");
  if (!cJSON_IsObject(camera)) {
    fprintf(stderr, "camera must exist and be an object\n");
    success = false;
    goto end;
  }

  if (!(success = camera_load(camera, &world->screen, &world->camera))) {
    goto end;
  }

  // objects
  cJSON *objects = cJSON_GetObjectItemCaseSensitive(json, "objects");
  if (!cJSON_IsArray(objects)) {
    fprintf(stderr, "objects must be an array of json objects\n");
    success = false;
    goto end;
  }

  // allocate space for objects
  world->num_objects = (size_t)cJSON_GetArraySize(objects);
  world->objects = malloc(sizeof(*world->objects) * world->num_objects);

  if (!(success = objects_load(objects, world->objects))) {
    goto end;
  }

  // UNFINISHED!!!
  success = false;
  goto end;

end:
  cJSON_Delete(json);
  return success;
}
