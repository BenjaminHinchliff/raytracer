srcs = files([
  'camera.c',
  'color.c',
  'hittable.c',
  'main.c',
  'material.c',
  'png_write.c',
  'ray.c',
  'util.c',
  'image.c',
  # 'work_queue.c', # replaced by openmp
  'trace.c',
])

ctracerexe = executable(
  'ctracer',
  srcs,
  include_directories : inc,
  dependencies : [
    m_dep,
    # threads_dep
    openmp_dep,
    libpng_dep,
    cglm_dep
  ],
)
