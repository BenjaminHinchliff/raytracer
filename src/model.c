#include "model.h"

#include <tinyobj_loader_c.h>

#include <stdio.h>
#include <stdlib.h>

void load_file_helper(void *ctx, const char *filename, const int is_mtl,
                      const char *obj_filename, char **buffer, size_t *len) {
  // supress warnings
  (void)ctx;
  (void)obj_filename;
  (void)is_mtl;

  long string_size = 0, read_size = 0;
  FILE *handler = fopen(filename, "r");

  if (handler) {
    fseek(handler, 0, SEEK_END);
    string_size = ftell(handler);
    rewind(handler);
    *buffer = (char *)malloc(sizeof(char) * (string_size + 1));
    read_size = fread(*buffer, sizeof(char), (size_t)string_size, handler);
    (*buffer)[string_size] = '\0';
    if (string_size != read_size) {
      free(buffer);
      *buffer = NULL;
    }
    fclose(handler);
  }

  *len = read_size;
}

void vertex_to_vec4(float *vtx, vec4 v) {
  glm_vec4_copy3(vtx, v);
  v[3] = 0.0;
}

bool model_load(const char *path, Model *model) {
  tinyobj_shape_t *shape = NULL;
  tinyobj_material_t *material = NULL;
  tinyobj_attrib_t attrib;

  size_t num_shapes;
  size_t num_materials;

  tinyobj_attrib_init(&attrib);

  int result =
      tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials,
                        path, load_file_helper, NULL, TINYOBJ_FLAG_TRIANGULATE);
  if (result != TINYOBJ_SUCCESS) {
    fprintf(stderr, "filed to load cube.obj\n");
    return false;
  }

  model->num_tris = attrib.num_face_num_verts;
  model->tris = malloc(sizeof(*model->tris) * model->num_tris);
  if (model->tris == NULL) {
    tinyobj_attrib_free(&attrib);
    return false;
  }

  for (unsigned int i = 0; i < attrib.num_face_num_verts; i++) {
    unsigned int vi = i * 3;

    Triangle tri;

    const tinyobj_vertex_index_t *face1 = &attrib.faces[vi];
    vertex_to_vec4(&attrib.vertices[face1->v_idx * 3], tri.v0);

    const tinyobj_vertex_index_t *face2 = &attrib.faces[vi + 1];
    vertex_to_vec4(&attrib.vertices[face2->v_idx * 3], tri.v1);

    const tinyobj_vertex_index_t *face3 = &attrib.faces[vi + 2];
    vertex_to_vec4(&attrib.vertices[face3->v_idx * 3], tri.v2);

    vertex_to_vec4(&attrib.normals[face1->vn_idx * 3], tri.normal);

    model->tris[i] = tri;
  }

  tinyobj_attrib_free(&attrib);
  return true;
}

bool model_hit(Model model, Ray ray, double t_min, double t_max,
               const Material *material, HitRecord *rec) {
  HitRecord tmp_rec;
  bool hit = false;
  double closest = t_max;

  for (size_t i = 0; i < model.num_tris; i++) {
    if (triangle_hit(model.tris[i], ray, t_min, closest, material, &tmp_rec)) {
      hit = true;
      closest = tmp_rec.t;
      *rec = tmp_rec;
    }
  }

  return hit;
}

void model_free(Model *model) { free(model->tris); }
