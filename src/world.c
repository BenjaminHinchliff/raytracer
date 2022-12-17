#include "world.h"
#include "camera.h"
#include "hittable.h"
#include "material.h"

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

bool mat_lambertian_load(cJSON *json_lam, Material *material) {
  material->type = MATERIAL_TYPE_lambertian;

  cJSON *albedo = cJSON_GetObjectItemCaseSensitive(json_lam, "albedo");
  CJSON_ASSERT_ARRAY(albedo);
  vec4_load(albedo, material->l_albedo);

  return true;
}

bool mat_dielectric_load(cJSON *json_di, Material *material) {
  material->type = MATERIAL_TYPE_dielectric;

  cJSON *ir = cJSON_GetObjectItemCaseSensitive(json_di, "index-of-refraction");
  CJSON_ASSERT_NUMBER(ir);

  material->ir = ir->valuedouble;

  return true;
}

bool mat_metal_load(cJSON *json_mtl, Material *material) {
  material->type = MATERIAL_TYPE_metal;

  cJSON *albedo = cJSON_GetObjectItemCaseSensitive(json_mtl, "albedo");
  CJSON_ASSERT_ARRAY(albedo);
  vec4_load(albedo, material->m_albedo);

  cJSON *fuzz = cJSON_GetObjectItemCaseSensitive(json_mtl, "fuzz");
  CJSON_ASSERT_NUMBER(fuzz);
  material->fuzz = fuzz->valuedouble;

  return true;
}

typedef struct MatKey {
  const char *key;
  bool (*loader)(cJSON *, Material *);
} MatKey;

const MatKey MATERIAL_KEYS[] = {
    {"lambertian", mat_lambertian_load},
    {"dielectric", mat_dielectric_load},
    {"metal", mat_metal_load},
};
const size_t NUM_MAT_KEYS = sizeof(MATERIAL_KEYS) / sizeof(MatKey);

// mat names is used to associate materials when objects are loaded later
bool materials_load(cJSON *json_mats, char **mat_names, Material *materials) {
  size_t i = 0;
  cJSON *material;
  cJSON_ArrayForEach(material, json_mats) {
    mat_names[i] = material->string;

    size_t m;
    for (m = 0; m < NUM_MAT_KEYS; m++) {
      const MatKey *key = &MATERIAL_KEYS[m];

      // search through potential material types to find a match
      cJSON *mat_data = cJSON_GetObjectItemCaseSensitive(material, key->key);
      if (cJSON_IsObject(mat_data)) {
        if (!key->loader(mat_data, &materials[i])) {
          fprintf(stderr, "failed to load %s material", key->key);
          return false;
        }
        break;
      }
    }

    // no material found
    if (m == NUM_MAT_KEYS) {
      fprintf(stderr, "unkown material type\n");
      return false;
    }

    i++;
  }

  return true;
}

// hashmap could be used for better performance
Material *lookup_material(char **names, Material *materials,
                          size_t num_materials, const char *material) {
  for (size_t i = 0; i < num_materials; i++) {
    if (strcmp(names[i], material) == 0) {
      return &materials[i];
    }
  }

  return NULL;
}

bool sphere_load(cJSON *json_sph, Hittable *sphere, char **mat_names,
                 Material *materials, size_t num_mats) {
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
  sphere->material =
      lookup_material(mat_names, materials, num_mats, material->valuestring);

  return true;
}

bool objects_load(cJSON *json_objs, Hittable *objects, char **mat_names,
                  Material *materials, size_t num_mats) {
  size_t i = 0;
  cJSON *object;
  cJSON_ArrayForEach(object, json_objs) {
    cJSON *sphere = cJSON_GetObjectItemCaseSensitive(object, "sphere");
    if (cJSON_IsObject(sphere)) {
      if (!sphere_load(sphere, &objects[i++], mat_names, materials, num_mats)) {
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
  char **mat_names = NULL;

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

  // materials
  cJSON *materials = cJSON_GetObjectItemCaseSensitive(json, "materials");
  if (!cJSON_IsObject(materials)) {
    fprintf(stderr, "materials must be stored in an object\n");
    success = false;
    goto end;
  }

  // allocate materials
  size_t num_mats = cJSON_GetArraySize(materials);
  mat_names = malloc(sizeof(*mat_names) * num_mats);
  if (mat_names == NULL) {
    fprintf(stderr, "failed to allocate space for material names\n");
    success = false;
    goto end;
  }
  world->materials = malloc(sizeof(*world->materials) * num_mats);
  if (world->materials == NULL) {
    fprintf(stderr, "failed to allocate space for materials\n");
    success = false;
    goto end;
  }

  if (!(success = materials_load(materials, mat_names, world->materials))) {
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
  if (world->objects == NULL) {
    fprintf(stderr, "failed to allocate space for objects\n");
    success = false;
    goto end;
  }

  if (!(success = objects_load(objects, world->objects, mat_names,
                               world->materials, num_mats))) {
    goto end;
  }

end:
  free(mat_names);
  cJSON_Delete(json);
  return success;
}
