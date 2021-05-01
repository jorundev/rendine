#include <rendine/Shader.hpp>
#include <rendine/utils/Log.hpp>
#include <glad/glad.h>
#include <spirv_cross/spirv_glsl.hpp>
#include <spirv_cross/spirv_cross_c.h>
#include <cerrno>
#include <cstring>
#include <exception>

void error_callback(void* data, const char* error)
{
	LOG_ERR(error);
}

namespace rendine {

static ShaderVarType sptype_to_shdtype(spvc_basetype type, unsigned int dim)
{
	switch (type) {
		case SPVC_BASETYPE_FP32:
			switch (dim) {
				case 1:
					return ShaderVarType::Float;
				case 2:
					return ShaderVarType::Vec2;
				case 3:
					return ShaderVarType::Vec3;
				case 4:
					return ShaderVarType::Vec4;
				default:
					return ShaderVarType::Unknown;
			}
		default:
			return ShaderVarType::Unknown;
	}
}

Shader::~Shader()
{
	this->unload();
}

void Shader::unload()
{
	if (this->is_loaded) {

		this->is_loaded = false;
		this->name.clear();
		free(this->glsl_source);
		this->glsl_source = nullptr;
		this->stage = ShaderStage::Unknown;
		glDeleteShader(this->getHandle());
	}
}

Result<void, const char *> Shader::loadFromFile(const char *filename, ShaderStage stage)
{
	FILE	*fp = fopen(filename, "rb");

	this->stage = stage;
	if (fp == nullptr) {
		return Err( (const char *)std::strerror(errno) );
	}

	fseek(fp, 0, SEEK_END);
	long source_size = ftell(fp);
	rewind(fp);

	uint32_t *spvs = (uint32_t *)malloc(source_size);
	if (spvs == nullptr) {
		fclose(fp);
		return Err( "Not Enough RAM" );
	}

	fread(spvs, 1, source_size, fp);
	fclose(fp);

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

	if (context == nullptr) {
		free(spvs);
		return Err( "Could not create SPIRV-Cross Context" );
	}

	// Set debug callback.
	spvc_context_set_error_callback(context, error_callback, 0);
	// Parse the SPIR-V.
	spvc_context_parse_spirv(context, spvs, source_size / 4, &ir);

	if (ir == nullptr) {
		free(spvs);
		spvc_context_destroy(context);
		return Err( "Could not create SPIRV-Cross IR" );
	}

	spvc_context_create_compiler(context, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &compiler_glsl);

	if (compiler_glsl == NULL) {
		free(spvs);
		spvc_context_destroy(context);
		return Err( "Could not create SPIRV-Cross GLSL Compiler" );
	}

	spvc_compiler_create_shader_resources(compiler_glsl, &resources);
	spvc_resources_get_resource_list_for_type(resources, SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, &list, &count);

	for (i = 0; i < count; i++)
	{
		if (strncmp(list[i].name, "Material", 9) == 0) {
			spvc_compiler_set_name(compiler_glsl, list[i].id, "material");
			spvc_type type = spvc_compiler_get_type_handle(compiler_glsl, list[i].base_type_id);
			this->material_uniforms.resize(spvc_type_get_vector_size(type));
			spvc_type_id id = spvc_type_get_base_type_id(type);

			unsigned int n = 0;
			while (n < this->material_uniforms.size()) {
				spvc_type_id member_type_id = spvc_type_get_member_type(type, n);
				spvc_type member_type = spvc_compiler_get_type_handle(compiler_glsl, member_type_id);
				spvc_basetype member_basetype = spvc_type_get_basetype(member_type);
				unsigned int member_dimension = spvc_type_get_vector_size(member_type);
				this->material_uniforms[n].name = std::string(spvc_compiler_get_member_name(compiler_glsl, id, n));
				this->material_uniforms[n].type = sptype_to_shdtype(member_basetype, member_dimension);
				++n;
			}
			break ;
		}
	}

	spvc_compiler_create_compiler_options(compiler_glsl, &options);
	spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 410);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, SPVC_FALSE);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ENABLE_420PACK_EXTENSION, SPVC_FALSE);
	spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_EMIT_UNIFORM_BUFFER_AS_PLAIN_UNIFORMS, SPVC_TRUE);
	spvc_compiler_install_compiler_options(compiler_glsl, options);

	const char *tmp;
	spvc_compiler_compile(compiler_glsl, &tmp);
	this->glsl_source = strdup(tmp);

	spvc_context_destroy(context);
	free(spvs);

	return compile_source();
}

Result<void, const char *>	Shader::compile_source()
{
	this->setHandle(glCreateShader((GLenum)this->stage));
	glShaderSource(this->getHandle(), 1, &this->glsl_source, NULL);
	glCompileShader(this->getHandle());

	int		success;
	char	infoLog[512];
	glGetShaderiv(this->getHandle(), GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(this->getHandle(), 512, NULL, infoLog);
		LOG_ERR("\n" << infoLog);
		this->unload();
		return Err( "Shader compilation failed" );
	}

	this->is_loaded = true;
	return Ok();
}

}
