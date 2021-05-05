#include <rendine/ShaderProgram.hpp>
#include <rendine/utils/Log.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	const auto& frag_mat_uniforms = this->fragment_shader->getMaterialUniforms();
	const auto& vert_mat_uniforms = this->vertex_shader->getMaterialUniforms();
	const auto& frag_cam_uniforms = this->fragment_shader->getCameraUniforms();
	const auto& vert_cam_uniforms = this->vertex_shader->getCameraUniforms();

	if (vert_mat_uniforms.size() != frag_mat_uniforms.size()) {
		LOG_ERR("Vertex and Fragment shader material uniforms are different:\n"
				<< "Vertex shader has " << vert_mat_uniforms.size() << " uniforms while Fragment shader has "
				<< frag_mat_uniforms.size() << ".");
		return Err( "Shader program linkage failed" );
	}

	if (vert_cam_uniforms.size() != frag_cam_uniforms.size()) {
		LOG_ERR("Vertex and Fragment shader camera uniforms are different:\n"
				<< "Vertex shader has " << vert_cam_uniforms.size() << " uniforms while Fragment shader has "
				<< frag_cam_uniforms.size() << ".");
		return Err( "Shader program linkage failed" );
	}

	//todo: very repetitive code
	for (int i = 0; i < frag_mat_uniforms.size(); ++i) {
		const auto& frag_mat_uniform = frag_mat_uniforms[i];
		const auto& vert_mat_uniform = vert_mat_uniforms[i];
		if (frag_mat_uniform.name != vert_mat_uniform.name
			|| frag_mat_uniform.type != vert_mat_uniform.type) {
			LOG_ERR("Vertex and Fragment shader material uniforms are different:\n"
				<< "Vertex shader's '" << vert_mat_uniform.name << "' differs from Fragment shader's '"
				<< frag_mat_uniform.name << "' in name or type");
			LOG_INFO("fragment generated GLSL:\n" << this->fragment_shader->getGLSLSource());
			LOG_INFO("vertex generated GLSL:\n" << this->vertex_shader->getGLSLSource());
			return Err( "Shader program linkage failed" );
		}
	}

	for (int i = 0; i < frag_cam_uniforms.size(); ++i) {
		const auto& frag_cam_uniform = frag_cam_uniforms[i];
		const auto& vert_cam_uniform = vert_cam_uniforms[i];
		if (frag_cam_uniform.name != vert_cam_uniform.name
			|| frag_cam_uniform.type != vert_cam_uniform.type) {
			LOG_ERR("Vertex and Fragment shader camera uniforms are different:\n"
				<< "Vertex shader's '" << vert_cam_uniform.name << "' differs from Fragment shader's '"
				<< frag_cam_uniform.name << "' in name or type");
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

	for (const auto& mat_uniform : vert_mat_uniforms) {
		this->material_uniforms[mat_uniform.name].location =
			glGetUniformLocation(this->getHandle(), mat_uniform.name.c_str());
		this->material_uniforms[mat_uniform.name].type = mat_uniform.type;
	}

	for (const auto& cam_uniform : vert_cam_uniforms) {
		this->camera_uniforms[cam_uniform.name].location =
			glGetUniformLocation(this->getHandle(), cam_uniform.name.c_str());
		this->camera_uniforms[cam_uniform.name].type = cam_uniform.type;
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

bool	ShaderProgram::setUniform(std::string name, glm::vec4 vec)
{
	const UniformLocationTypePair *uniform = nullptr;
	if (this->material_uniforms.find(name) == this->material_uniforms.end()) {
		if (this->camera_uniforms.find(name) == this->camera_uniforms.end()) {
			return false;
		} else {
			uniform = &camera_uniforms[name];
		}
	} else {
		uniform = &material_uniforms[name];
	}
	if (uniform->type == ShaderVarType::Vec4) {
		glUniform4f(uniform->location, vec.x, vec.y, vec.z, vec.w);
		return true;
	}
	return false;
}

// todo: (and more)
/*bool	ShaderProgram::setMaterialUniform(std::string name, glm::vec3 vec);
bool	ShaderProgram::setMaterialUniform(std::string name, glm::vec2 vec);
bool	ShaderProgram::setMaterialUniform(std::string name, float num);*/

bool	ShaderProgram::setUniform(std::string name, glm::mat4 mat)
{
	const UniformLocationTypePair *uniform = nullptr;
	if (this->material_uniforms.find(name) == this->material_uniforms.end()) {
		if (this->camera_uniforms.find(name) == this->camera_uniforms.end()) {
			return false;
		} else {
			uniform = &camera_uniforms[name];
		}
	} else {
		uniform = &material_uniforms[name];
	}
	if (uniform->type == ShaderVarType::Mat4) {
		glUniformMatrix4fv(uniform->location, 1, GL_FALSE, glm::value_ptr(mat));
		return true;
	}
	return false;
}

}
