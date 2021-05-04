#include <rendine/meshes/Mesh2D.hpp>
#include <cstddef>

using namespace glm;

namespace rendine
{

Mesh2D::Mesh2D(): Mesh()
{
	this->bind();
}

void	Mesh2D::addVertex(vec2 position, vec4 color, vec2 uvCoord)
{
	Vertex vertex = {
		.position = position,
		.color = color,
		.uvCoord = uvCoord,
	};
	addVertex(vertex);
}

void	Mesh2D::addVertex(const Vertex& vertex)
{
	this->vertices.push_back(vertex);
	this->indices.push_back(this->indices.size());
}

void	Mesh2D::bake()
{
	this->bind();
	glBufferData(	GL_ARRAY_BUFFER,
					this->vertices.size() * sizeof(Vertex),
					&this->vertices[0],
					GL_STATIC_DRAW		);

	glBufferData(	GL_ELEMENT_ARRAY_BUFFER,
					this->indices.size() * sizeof(unsigned int),
					&this->indices[0],
					GL_STATIC_DRAW		);

	/* Positions */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	/* Color */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

	/* UV Coords */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uvCoord));

	/* Unbind VAO */
	glBindVertexArray(0);
}

}
