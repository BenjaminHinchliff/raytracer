project('ctracer', 'c',
  version : '1.0.0',
  license : 'MIT',
  default_options : ['warning_level=3', 'werror=true', 'b_lto=true', 'c_std=c11'])

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required: false)
threads_dep = dependency('threads')
openmp_dep = dependency('openmp')

libpng_dep = dependency('libpng')
cglm_dep = dependency('cglm', fallback : ['cglm', 'cglm_dep'])

inc = include_directories('include')

subdir('src')
