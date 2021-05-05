#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"

namespace rendine
{

class RenderObject {
public:
	RenderObject(Mesh& mesh);

private:
	Mesh&		mesh;
	Transform	transform;

};

}
