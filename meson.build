project('mc', 'c', 'cpp')

include = include_directories (
	'include',
)

glad = static_library (
	'glad', 'vendor/glad/glad.c',
	include_directories: ['vendor/glad/include/', 'vendor/KHR/include/']
)

glad_dep = declare_dependency (
	link_with: glad,
	include_directories: ['vendor/glad/include/', 'vendor/KHR/include/']
)

cglm = static_library (
	'cglm', [
	'vendor/cglm/src/quat.c',
	'vendor/cglm/src/ease.c',
	'vendor/cglm/src/vec4.c',
	'vendor/cglm/src/affine.c',
	'vendor/cglm/src/mat2.c',
	'vendor/cglm/src/curve.c',
	'vendor/cglm/src/ray.c',
	'vendor/cglm/src/io.c',
	'vendor/cglm/src/affine2d.c',
	'vendor/cglm/src/vec3.c',
	'vendor/cglm/src/project.c',
	'vendor/cglm/src/euler.c',
	'vendor/cglm/src/mat4.c',
	'vendor/cglm/src/box.c',
	'vendor/cglm/src/vec2.c',
	'vendor/cglm/src/cam.c',
	'vendor/cglm/src/bezier.c',
	'vendor/cglm/src/sphere.c',
	'vendor/cglm/src/frustum.c',
	'vendor/cglm/src/mat3.c',
	'vendor/cglm/src/plane.c'
],
)

cglm_dep = declare_dependency (
	link_with: cglm,
	include_directories: ['vendor/cglm/include']
)

spvcross = static_library (
	'spirvcross', [
'vendor/SPIRV-Cross/spirv_glsl.cpp',
'vendor/SPIRV-Cross/spirv_cfg.cpp',
'vendor/SPIRV-Cross/spirv_reflect.cpp',
'vendor/SPIRV-Cross/spirv_msl.cpp',
'vendor/SPIRV-Cross/spirv_cross_util.cpp',
'vendor/SPIRV-Cross/spirv_cross.cpp',
'vendor/SPIRV-Cross/spirv_cross_c.cpp',
'vendor/SPIRV-Cross/spirv_hlsl.cpp',
'vendor/SPIRV-Cross/spirv_cpp.cpp',
'vendor/SPIRV-Cross/spirv_cross_parsed_ir.cpp',
'vendor/SPIRV-Cross/spirv_parser.cpp',
],
include_directories: ['vendor/SPIRV-Cross/include'],
override_options : ['cpp_std=c++11', 'optimization=3'],
cpp_args: '-DSPIRV_CROSS_C_API_GLSL',
)

spvcross_dep = declare_dependency (
	link_with: spvcross,
	include_directories: ['vendor/SPIRV-Cross/include']
)

exe = executable (
	'exe', ['src/main.c', 'src/assets.c', 'src/texture.c', 'src/shader.c'],
	include_directories:[include],
	dependencies:[glad_dep, dependency('SDL2'), cglm_dep, dependency('libpng'), spvcross_dep]
)
