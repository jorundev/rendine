#pragma once

#include <types.h>
#include <png.h>
#include <glad/glad.h>

typedef
struct texture_s {
	size2			size;
	unsigned char	color_type;
	unsigned char	bit_depth;
	unsigned char	*raw;
	GLuint			gl_obj;

}	texture_t;

const char *png_get_error(void);
const char *png_get_error_function(void);

bool texture_png_load_from_file(texture_t *tex, const char *filename);
