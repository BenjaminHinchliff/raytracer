project('ctracer', 'c',
  version : '1.0.0',
  license : 'MIT')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required: false)

inc = include_directories('include')

subdir('src')
