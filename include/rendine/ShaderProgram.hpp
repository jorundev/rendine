#pragma once

#include <unordered_map>
#include "Asset.hpp"
#include "GLObject.hpp"
#include "Shader.hpp"
#include "utils/Result.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rendine {

using namespace std;

struct UniformLocationTypePair {
	GLint			location;
	ShaderVarType	type;
};

class ShaderProgram : public Asset, public GLObject {
public:
	ShaderProgram();

	~ShaderProgram() override;

	Result<void, const char *>	link();

	void	attachShader(std::shared_ptr<Shader> shader);
	void	use() const;

	bool	setUniform(std::string name, glm::vec4 vec);
	bool	setUniform(std::string name, glm::vec3 vec);
	bool	setUniform(std::string name, glm::vec2 vec);
	bool	setUniform(std::string name, float num);
	bool	setUniform(std::string name, glm::mat4 mat);

	virtual void	unload() override;
private:
	std::shared_ptr<Shader>	vertex_shader;
	std::shared_ptr<Shader>	fragment_shader;
	unordered_map<string, UniformLocationTypePair> material_uniforms;
	unordered_map<string, UniformLocationTypePair> camera_uniforms;

	void attach(std::shared_ptr<Shader>& shader);

};

}
