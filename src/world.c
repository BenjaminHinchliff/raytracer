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

bool camera_load(cJSON *json_cam, const Screen *screen, Camera *camera) {
  cJSON *j_eye = cJSON_GetObjectItemCaseSensitive(json_cam, "eye");
  CJSON_ASSERT_ARRAY(j_eye);
  vec4 eye;
  vec4_load(j_eye, eye);

  cJSON *j_center = cJSON_GetObjectItemCaseSensitive(json_cam, "center");
  CJSON_ASSERT_ARRAY(j_center);
  vec4 center;
  vec4_load(j_center, center);

  cJSON *j_up = cJSON_GetObjectItemCaseSensitive(json_cam, "up");
  CJSON_ASSERT_ARRAY(j_up);
  vec4 up;
  vec4_load(j_up, up);

  cJSON *j_height = cJSON_GetObjectItemCaseSensitive(json_cam, "height");
  CJSON_ASSERT_NUMBER(j_height);
  double height = j_height->valuedouble;

  cJSON *j_vfov = cJSON_GetObjectItemCaseSensitive(json_cam, "vfov");
  CJSON_ASSERT_NUMBER(j_vfov);
  double vfov = j_vfov->valuedouble;

  cJSON *width = cJSON_GetObjectItemCaseSensitive(json_cam, "width");
  if (cJSON_IsString(width) && strcmp(width->valuestring, "auto") == 0) {
    double aspect = (double)screen->width / (double)screen->height;
    double width = height * aspect;
    *camera = camera_new(eye, center, up, height, width, vfov);
  } else if (cJSON_IsNumber(width)) {
    *camera = camera_new(eye, center, up, height, width->valuedouble, vfov);
  } else {
    fprintf(stderr, "camera width must be a number or \"auto\"\n");
    return false;
  }

  return true;
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
          fprintf(stderr, "failed to load %s material\n", key->key);
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
  sphere->material =
      lookup_material(mat_names, materials, num_mats, material->valuestring);

  return true;
}

bool triangle_load(cJSON *json_tri, Hittable *tri, char **mat_names,
                   Material *materials, size_t num_mats) {
  tri->type = HITTABLE_TYPE_triangle;

  cJSON *v0 = cJSON_GetObjectItemCaseSensitive(json_tri, "v0");
  CJSON_ASSERT_ARRAY(v0);
  if (!vec4_load(v0, tri->v0)) {
    return false;
  }

  cJSON *v1 = cJSON_GetObjectItemCaseSensitive(json_tri, "v1");
  CJSON_ASSERT_ARRAY(v1);
  if (!vec4_load(v1, tri->v1)) {
    return false;
  }

  cJSON *v2 = cJSON_GetObjectItemCaseSensitive(json_tri, "v2");
  CJSON_ASSERT_ARRAY(v2);
  if (!vec4_load(v2, tri->v2)) {
    return false;
  }

  cJSON *normal = cJSON_GetObjectItemCaseSensitive(json_tri, "normal");
  CJSON_ASSERT_ARRAY(normal);
  if (!vec4_load(normal, tri->normal)) {
    return false;
  }

  cJSON *material = cJSON_GetObjectItemCaseSensitive(json_tri, "material");
  CJSON_ASSERT_STRING(material);
  tri->material =
      lookup_material(mat_names, materials, num_mats, material->valuestring);

  return true;
}

typedef struct ObjKey {
  const char *key;
  bool (*loader)(cJSON *, Hittable *, char **mat_names, Material *materials,
                 size_t num_mats);
} ObjKey;

const ObjKey OBJECT_KEYS[] = {
    {"sphere", sphere_load},
    {"triangle", triangle_load},
};
const size_t NUM_OBJECT_KEYS = sizeof(OBJECT_KEYS) / sizeof(ObjKey);

bool objects_load(cJSON *json_objs, Hittable *objects, char **mat_names,
                  Material *materials, size_t num_mats) {
  size_t i = 0;
  cJSON *object;
  cJSON_ArrayForEach(object, json_objs) {
    size_t o;
    for (o = 0; o < NUM_OBJECT_KEYS; o++) {
      const ObjKey *key = &OBJECT_KEYS[o];

      cJSON *obj_data = cJSON_GetObjectItemCaseSensitive(object, key->key);
      if (cJSON_IsObject(obj_data)) {
        if (!key->loader(obj_data, &objects[i], mat_names, materials,
                         num_mats)) {
          fprintf(stderr, "failed to load %s object\n", key->key);
          return false;
        }
        if (objects[i].material == NULL) {
          fprintf(stderr, "failed to load material\n");
          return false;
        }
        break;
      }
    }

    if (o == NUM_OBJECT_KEYS) {
      fprintf(stderr, "unknown object type\n");
      return false;
    }

    i++;
  }

  return true;
}

bool world_load(const char *json_src, World **world_ptr) {
  bool success = true;
  World *world = NULL;
  cJSON *json = NULL;
  char **mat_names = NULL;

  world = malloc(sizeof(*world));
  if (world == NULL) {
    fprintf(stderr, "failed to allocate memory for world\n");
    success = false;
    goto err;
  }

  // parse world to json object
  json = cJSON_Parse(json_src);
  if (json == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    success = false;
    goto err;
  }

  // screen
  cJSON *screen = cJSON_GetObjectItemCaseSensitive(json, "screen");
  if (!cJSON_IsObject(screen)) {
    fprintf(stderr, "screen must exist and be an object\n");
    success = false;
    goto err;
  }

  success = screen_load(screen, &world->screen);
  if (!success) {
    goto err;
  }

  // camera
  cJSON *camera = cJSON_GetObjectItemCaseSensitive(json, "camera");
  if (!cJSON_IsObject(camera)) {
    fprintf(stderr, "camera must exist and be an object\n");
    success = false;
    goto err;
  }

  success = camera_load(camera, &world->screen, &world->camera);
  if (!success) {
    goto err;
  }

  // materials
  cJSON *materials = cJSON_GetObjectItemCaseSensitive(json, "materials");
  if (!cJSON_IsObject(materials)) {
    fprintf(stderr, "materials must be stored in an object\n");
    success = false;
    goto err;
  }

  // allocate materials
  size_t num_mats = cJSON_GetArraySize(materials);
  mat_names = malloc(sizeof(*mat_names) * num_mats);
  if (mat_names == NULL) {
    fprintf(stderr, "failed to allocate space for material names\n");
    success = false;
    goto err;
  }
  world->materials = malloc(sizeof(*world->materials) * num_mats);
  if (world->materials == NULL) {
    fprintf(stderr, "failed to allocate space for materials\n");
    success = false;
    goto err;
  }

  success = materials_load(materials, mat_names, world->materials);
  if (!success) {
    goto err;
  }

  // objects
  cJSON *objects = cJSON_GetObjectItemCaseSensitive(json, "objects");
  if (!cJSON_IsArray(objects)) {
    fprintf(stderr, "objects must be an array of json objects\n");
    success = false;
    goto err;
  }

  // allocate space for objects
  world->num_objects = (size_t)cJSON_GetArraySize(objects);
  world->objects = malloc(sizeof(*world->objects) * world->num_objects);
  if (world->objects == NULL) {
    fprintf(stderr, "failed to allocate space for objects\n");
    success = false;
    goto err;
  }

  success = objects_load(objects, world->objects, mat_names, world->materials,
                         num_mats);
  if (!success) {
    goto err;
  }

  *world_ptr = world;
  goto end;

err:
  free(world);
end:
  free(mat_names);
  cJSON_Delete(json);
  return success;
}

void world_free(World *world) {
  if (world == NULL) {
    return;
  }

  free(world->objects);
  free(world->materials);
  free(world);
}
