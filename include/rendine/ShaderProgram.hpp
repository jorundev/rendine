#pragma once

#include <unordered_map>
#include <utility>
#include "Asset.hpp"
#include "GLObject.hpp"
#include "Shader.hpp"
#include "utils/Result.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rendine {

class ShaderProgram : public Asset, public GLObject {
public:
	ShaderProgram();

	~ShaderProgram() override;

	Result<void, const char *>	link();

	void	attachShader(std::shared_ptr<Shader> shader);
	void	use() const;

	bool	setMaterialUniform(std::string name, glm::vec4 vec);
	bool	setMaterialUniform(std::string name, glm::vec3 vec);
	bool	setMaterialUniform(std::string name, glm::vec2 vec);
	bool	setMaterialUniform(std::string name, float num);

	virtual void	unload() override;
private:
	std::shared_ptr<Shader>	vertex_shader;
	std::shared_ptr<Shader>	fragment_shader;
	std::unordered_map<std::string, std::pair<GLint, ShaderVarType>> material_uniforms;

	void attach(std::shared_ptr<Shader>& shader);

};

}
