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

spvcross = static_library (
	'spirvcross', [
'vendor/SPIRV-Cross/spirv_glsl.cpp',
'vendor/SPIRV-Cross/spirv_cfg.cpp',
'vendor/SPIRV-Cross/spirv_reflect.cpp',
'vendor/SPIRV-Cross/spirv_cross_util.cpp',
'vendor/SPIRV-Cross/spirv_cross.cpp',
'vendor/SPIRV-Cross/spirv_cross_c.cpp',
'vendor/SPIRV-Cross/spirv_cross_parsed_ir.cpp',
'vendor/SPIRV-Cross/spirv_parser.cpp',
],
include_directories: ['vendor/SPIRV-Cross', 'vendor/SPIRV-Cross/include'],
override_options : ['cpp_std=c++11'],
cpp_args: '-DSPIRV_CROSS_C_API_GLSL',
)

spvcross_dep = declare_dependency (
	link_with: spvcross,
	include_directories: ['vendor/SPIRV-Cross/include']
)

exe = executable (
	'exe', [	'src/main.cpp',
				'src/Texture.cpp',
				'src/Shader.cpp',
				'src/ShaderProgram.cpp',
				'src/Mesh.cpp',
				'src/meshes/Mesh2D.cpp'
			],
	override_options : ['cpp_std=c++17'],
	include_directories:[include],
	dependencies:[glad_dep, dependency('SDL2'), dependency('glm'), dependency('libpng'), spvcross_dep]
)
