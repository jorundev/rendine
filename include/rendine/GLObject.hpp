#pragma once

#include <glad/glad.h>

namespace rendine {

class GLObject {
public:
	void	setHandle(GLuint handle) { this->handle = handle; }
	GLuint	getHandle() const { return this->handle; }
	GLuint	*getHandlePtr() { return &this->handle; }
private:
	GLuint	handle;
};

}
