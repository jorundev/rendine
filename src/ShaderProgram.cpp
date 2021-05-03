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

	const auto& frag_uniforms = this->fragment_shader->getUniforms();
	const auto& vert_uniforms = this->vertex_shader->getUniforms();

	if (frag_uniforms.size() != vert_uniforms.size()) {
		LOG_ERR("Vertex and Fragment shader material uniforms are different\n"
			"Vertex shader has " << vert_uniforms.size() << " uniforms. Fragment shader has " << frag_uniforms.size());
		LOG_INFO("fragment generated GLSL:\n" << this->fragment_shader->getGLSLSource());
		LOG_INFO("vertex generated GLSL:\n" << this->vertex_shader->getGLSLSource());
		return Err( "Shader program linkage failed" );
	}
	for (int i = 0; i < frag_uniforms.size(); ++i) {
		const auto& frag_mat_uniform = frag_uniforms[i];
		const auto& vert_mat_uniform = vert_uniforms[i];
		if (frag_mat_uniform.name != vert_mat_uniform.name
			|| frag_mat_uniform.type != vert_mat_uniform.type) {
			LOG_ERR("Vertex and Fragment shader material uniforms are different\n"
				<< "Vertex shader's '" << vert_mat_uniform.name << "' differs from Fragment shader's '"
				<< frag_mat_uniform.name << "' in name or type");
			LOG_INFO("fragment generated GLSL:\n" << this->fragment_shader->getGLSLSource());
			LOG_INFO("vertex generated GLSL:\n" << this->vertex_shader->getGLSLSource());
			return Err( "Shader program linkage failed" );
		}
	}

	glLinkProgram(this->getHandle());

	int		success;
	char	infoLog[512];

	glGetProgramiv(this->getHandle(), GL_LINK_STATUS, &success);
	if(!success) {
		// Program Linkage Error
		glGetProgramInfoLog(this->getHandle(), 512, NULL, infoLog);
		LOG_ERR("\n" << infoLog);
		LOG_INFO("fragment generated GLSL:\n" << this->fragment_shader->getGLSLSource());
		LOG_INFO("vertex generated GLSL:\n" << this->vertex_shader->getGLSLSource());
		this->unload();
		return Err( "Shader program linkage failed" );
	}

	this->use();

	for (const auto& mat_uniform : vert_uniforms) {
		std::string mat_name = std::string("material.") + mat_uniform.name;
		this->material_uniforms[mat_uniform.name].first =
			glGetUniformLocation(this->getHandle(), mat_name.c_str());
		this->material_uniforms[mat_uniform.name].second = mat_uniform.type;
	}

	this->is_loaded = true;
	return Ok();
}

void	ShaderProgram::use() const
{
	if (this->is_loaded) {
		glUseProgram(this->getHandle());
	}
}

bool	ShaderProgram::setMaterialUniform(std::string name, glm::vec4 vec)
{
	if (this->material_uniforms.find(name) == this->material_uniforms.end()) {
		return false;
	}
	const auto& uniform = this->material_uniforms[name];
	if (uniform.second == ShaderVarType::Vec4) {
		glUniform4f(uniform.first, vec.x, vec.y, vec.z, vec.w);
		return true;
	}
	return false;
}

// todo: (and more)
/*bool	ShaderProgram::setMaterialUniform(std::string name, glm::vec3 vec);
bool	ShaderProgram::setMaterialUniform(std::string name, glm::vec2 vec);
bool	ShaderProgram::setMaterialUniform(std::string name, float num);*/
}
