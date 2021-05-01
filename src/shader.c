#include "shader.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <spirv_cross/spirv_cross_c.h>

const char	*g_shader_error_function = "No Error";
const char	*g_shader_error = "No Error";

const char *shader_get_error(void)
{
	return g_shader_error;
}

const char *shader_get_error_function(void)
{
	return g_shader_error_function;
}

static void error_callback(void* data, const char* error)
{
  printf("SPIRV-Cross: %s\n", error);
}

static shader_type_t sptype_to_shdtype(spvc_basetype type, unsigned int dim)
{
	switch (type) {
		case SPVC_BASETYPE_FP32:
			switch (dim) {
				case 1:
					return SHADER_TYPE_FLOAT;
				case 2:
					return SHADER_TYPE_FLOAT2;
				case 3:
					return SHADER_TYPE_FLOAT3;
				case 4:
					return SHADER_TYPE_FLOAT4;
				default:
					return SHADER_TYPE_UNKNOWN;
			}
		default:
			return SHADER_TYPE_UNKNOWN;
	}
}

bool shader_load_from_spirv_file(shader_t *shdr, const char *filename)
{
	FILE			*fp = fopen(filename, "rb");

	if (!fp) {
		g_shader_error_function = "shader_load_from_spirv_file";
		g_shader_error = strerror(errno);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	long source_size = ftell(fp);
	rewind(fp);

	SpvId *spv = malloc(source_size);
	if (!spv) {
		g_shader_error = "Not Enough RAM";
		fclose(fp);
		goto error;
	}
	fread(spv, 1, source_size, fp);
	fclose(fp);

	size_t word_count = source_size / 4;

	spvc_context context = NULL;
	spvc_parsed_ir ir = NULL;
	spvc_compiler compiler_glsl = NULL;
	spvc_compiler_options options = NULL;
	spvc_resources resources = NULL;
	const spvc_reflected_resource *list = NULL;
	size_t count;
	size_t i;

	// Create context.
	spvc_context_create(&context);

	if (context == NULL) {
		g_shader_error = "Could not create SPIRV-Cross Context";
		goto error;
	}

	// Set debug callback.
	spvc_context_set_error_callback(context, error_callback, 0);
	// Parse the SPIR-V.
	spvc_context_parse_spirv(context, spv, word_count, &ir);

	if (ir == NULL) {
		g_shader_error = "Could not create SPIRV-Cross IR";
		goto error;
	}

	spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler_glsl);

	if (compiler_glsl == NULL) {
		g_shader_error = "Could not create SPIRV-Cross GLSL Compiler";
		goto error;
	}

	spvc_compiler_create_shader_resources(compiler_glsl, &resources);
	spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, &list, &count);

	for (i = 0; i < count; i++)
	{
		if (strncmp(list[i].name, "Material", 9) == 0) {
			spvc_compiler_set_name(compiler_glsl, list[i].id, "material");
			spvc_type type = spvc_compiler_get_type_handle(compiler_glsl, list[i].base_type_id);
			shdr->num_uniforms = spvc_type_get_vector_size(type);
			spvc_type_id id = spvc_type_get_base_type_id(type);

			shdr->uniform_names = malloc(shdr->num_uniforms);
			memset(shdr->uniform_names, 0, shdr->num_uniforms);
			shdr->uniform_types = malloc(shdr->num_uniforms);
			memset(shdr->uniform_types, 0, shdr->num_uniforms);
			unsigned int n = 0;
			while (n < shdr->num_uniforms) {
				spvc_type_id member_type_id = spvc_type_get_member_type(type, n);
				spvc_type member_type = spvc_compiler_get_type_handle(compiler_glsl, member_type_id);
				spvc_basetype member_basetype = spvc_type_get_basetype(member_type);
				unsigned int member_dimension = spvc_type_get_vector_size(member_type);
				shdr->uniform_names[n] = strdup(spvc_compiler_get_member_name(compiler_glsl, id, n));
				shdr->uniform_types[n] = sptype_to_shdtype(member_basetype, member_dimension);
				++n;
			}
			break ;
		}
	}

	spvc_compiler_create_compiler_options(compiler_glsl, &options);
	spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 410);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, SPVC_FALSE);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ENABLE_420PACK_EXTENSION, SPVC_FALSE);
	// spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_EMIT_UNIFORM_BUFFER_AS_PLAIN_UNIFORMS, SPVC_TRUE);
	spvc_compiler_install_compiler_options(compiler_glsl, options);

	const char *tmp;
	spvc_compiler_compile(compiler_glsl, &tmp);
	shdr->shader_source = strdup(tmp);

	// Frees all memory we allocated so far.
	spvc_context_destroy(context);

	free(spv);
	return true;

	error:	free(spv);
			g_shader_error_function = "shader_load_from_spirv_file";
			return false;
}
