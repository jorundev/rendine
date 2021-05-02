#include <rendine/ShaderProgram.hpp>
#include <rendine/utils/Log.hpp>

namespace rendine {

ShaderProgram::ShaderProgram()
	:	Asset()
{
	this->setHandle(glCreateProgram());
}

ShaderProgram::~ShaderProgram()
{
	this->unload();
}

void ShaderProgram::unload()
{
	if (this->is_loaded) {
		this->is_loaded = false;
		glDeleteProgram(this->getHandle());
	}
}

void	ShaderProgram::attachShader(std::shared_ptr<Shader> shader)
{
	if (shader->getStage() == ShaderStage::Vertex) {
		this->vertex_shader = std::move(shader);
		this->attach(this->vertex_shader);
	} else if (shader->getStage() == ShaderStage::Fragment) {
		this->fragment_shader = std::move(shader);
		this->attach(this->fragment_shader);
	}
}

void ShaderProgram::attach(std::shared_ptr<Shader>& shader)
{
	glAttachShader(this->getHandle(), shader->getHandle());
}

Result<void, const char *>	ShaderProgram::link()
{
	glLinkProgram(this->getHandle());


	int		success;
	char	infoLog[512];

	glGetProgramiv(this->getHandle(), GL_LINK_STATUS, &success);
	if(!success) {
		// Program Linkage Error
		glGetProgramInfoLog(this->getHandle(), 512, NULL, infoLog);
		LOG_ERR("\n" << infoLog);
		this->unload();
		return Err( "Shader program linkage failed" );
	}

	this->is_loaded = true;
	return Ok();
}

}
