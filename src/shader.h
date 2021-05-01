#pragma once

#include <types.h>

const char *shader_get_error(void);
const char *shader_get_error_function(void);

typedef
enum shader_type_s {
	SHADER_TYPE_UNKNOWN,
	SHADER_TYPE_FLOAT,
	SHADER_TYPE_FLOAT2,
	SHADER_TYPE_FLOAT3,
	SHADER_TYPE_FLOAT4,
	SHADER_TYPE_INT,
	SHADER_TYPE_INT2,
	SHADER_TYPE_INT3,
	SHADER_TYPE_INT4,
	SHADER_TYPE_UINT,
	SHADER_TYPE_UINT2,
	SHADER_TYPE_UINT3,
	SHADER_TYPE_UINT4,
}	shader_type_t;

typedef
struct shader_s {
	const char		*shader_source;
	unsigned int	num_uniforms;
	char			**uniform_names;
	shader_type_t	*uniform_types;
}	shader_t;

bool shader_load_from_spirv_file(shader_t *shdr, const char *filename);
