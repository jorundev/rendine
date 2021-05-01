#include <rendine/Shader.hpp>
#include <glad/glad.h>

namespace rendine {

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

}
