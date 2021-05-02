#pragma once

#include "Asset.hpp"
#include "GLObject.hpp"
#include "Shader.hpp"
#include "utils/Result.hpp"
#include <glad/glad.h>

namespace rendine {

class ShaderProgram : public Asset, public GLObject {
public:
	ShaderProgram();

	~ShaderProgram() override;

	Result<void, const char *>	link();

	void	attachShader(std::shared_ptr<Shader> shader);

	virtual void	unload() override;
private:
	std::shared_ptr<Shader>	vertex_shader;
	std::shared_ptr<Shader>	fragment_shader;

	void attach(std::shared_ptr<Shader>& shader);

};

}
