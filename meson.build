project('mc', 'c')

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
]
)

cglm_dep = declare_dependency (
	link_with: cglm,
	include_directories: ['vendor/cglm/include']
)

exe = executable (
	'exe', ['src/main.c', 'src/assets.c', 'src/texture.c'],
	include_directories:[include],
	dependencies:[glad_dep, dependency('SDL2'), cglm_dep, dependency('libpng')]
)
