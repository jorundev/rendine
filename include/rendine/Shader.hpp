#pragma once

#include "Asset.hpp"
#include "GLObject.hpp"
#include "utils/Result.hpp"
#include <glad/glad.h>

#include <vector>
#include <string>

namespace rendine {

enum class ShaderStage {
	Unknown = -1,
	Geometry = GL_GEOMETRY_SHADER,
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Compute = GL_COMPUTE_SHADER,
};

enum class ShaderVarType {
	Unknown,
	Bool,
	BVec2,
	BVec3,
	BVec4,
	Float,
	Vec2,
	Vec3,
	Vec4,
	Int,
	IVec2,
	IVec3,
	IVec4,
	UInt,
	UVec2,
	UVec3,
	UVec4,
	Double,
	DVec2,
	DVec3,
	DVec4,
	Mat4,
	Mat4x4 = Mat4,
	Mat3,
	Mat3x3 = Mat3,
	Mat2,
	Mat2x2 = Mat2,
};

struct ShaderUniform {
	std::string		name;
	ShaderVarType	type;
};


class Shader : public Asset, public GLObject {
public:
	Shader() :	Asset(),
				name(""),
				stage(ShaderStage::Unknown),
				glsl_source(nullptr) {};

	~Shader() override;

	virtual void	unload() override;
	Result<void, const char *> loadFromFile(const char *filename, ShaderStage stage);

	ShaderStage	getStage() const { return this->stage; };

	const std::vector<ShaderUniform>& getUniforms() const { return this->material_uniforms; }

private:
	std::string					name;
	ShaderStage					stage;
	char						*glsl_source;
	std::vector<ShaderUniform>	material_uniforms;

	Result<void, const char *>	compile_source();
};

}
