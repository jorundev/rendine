#pragma once

#include "../Mesh.hpp"
#include <glm/glm.hpp>

using namespace glm;

namespace rendine
{

class Mesh2D : public Mesh {

struct Vertex {
	vec2	position;
	vec4	color;
	vec2	uvCoord;
};

public:
	Mesh2D();
	void	addVertex(vec2 position, vec4 color, vec2 uvCoord);
	void	addVertex(const Mesh2D::Vertex& vertex);
	void	bake();

private:

	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;

};

}
