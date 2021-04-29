project('mc', 'c')

include = include_directories (
	'include',
)

glad = static_library (
	'glad', 'vendor/glad/glad.c',
	include_directories: ['vendor/glad/include/', 'vendor/KHR/include/']
)

glad_dep =  declare_dependency (
	link_with: glad,
	include_directories: ['vendor/glad/include/', 'vendor/KHR/include/']
)

exe = executable (
	'exe', ['src/main.c', 'src/assets.c', 'src/texture.c'],
	include_directories:[include],
	dependencies:[glad_dep, dependency('SDL2'), dependency('cglm'), dependency('libpng')]
)
