#pragma once

#include "Asset.hpp"
#include "GLObject.hpp"

#include <vector>
#include <string>

namespace rendine {

enum class ShaderStage {
	Unknown,
	Geometry,
	Vertex,
	Fragment,
	Tesselation,
	Compute,
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
private:
	std::string					name;
	ShaderStage					stage;
	unsigned char				*glsl_source;
	std::vector<ShaderUniform>	material_uniforms;
};

}
