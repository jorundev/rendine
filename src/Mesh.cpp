#include <rendine/Mesh.hpp>
#include <glad/glad.h>

namespace rendine
{

Mesh::Mesh()
{
	glGenBuffers(1, &this->ebo);
	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &this->ebo);
	glDeleteBuffers(1, &this->vbo);
	glDeleteVertexArrays(1, &this->vao);
}

void Mesh::bind() const
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
}

}
