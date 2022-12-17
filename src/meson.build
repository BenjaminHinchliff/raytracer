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
  'trace.c',
  'world.c',
])

opts = []
if get_option('buildtype') == 'debug'
  opts += ['b_sanitize=address,undefined', 'b_lundef=false']
endif

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
  ],
  override_options: opts,
)
