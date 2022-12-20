project('tinyobjloader-c', 'c')

tinyobjloader_c_dep = declare_dependency(include_directories : ['.'])

pkgconfig = import('pkgconfig')
pkgconfig.generate(
  name : 'tinyobjloader-c',
  subdirs : ['.'],
  description : 'small wavefront obj file loader',
)

