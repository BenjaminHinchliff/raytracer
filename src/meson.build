srcs = files([
  'camera.c',
  'color.c',
  'hit_record.c',
  'hittable.c',
  'hittable_list.c',
  'main.c',
  'material.c',
  'png_write.c',
  'ray.c',
  'util.c',
  'image.c',
  'trace.c',
  'world.c',
  'options.c',
  'triangle.c',
  'model.c',
])

ctracerexe = executable(
  'ctracer',
  srcs,
  include_directories : inc,
  dependencies : [
    m_dep,
    openmp_dep,
    libpng_dep,
    cglm_dep,
    cjson_dep,
    tinyobjloader_dep
  ],
)
