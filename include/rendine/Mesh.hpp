#pragma once

#include "GLObject.hpp"
#include <vector>

namespace rendine
{

class Mesh {
public:
	Mesh();
	~Mesh();
	void	bind() const;

protected:
	GLuint				ebo;
	GLuint				vao;
	GLuint				vbo;

};

}
