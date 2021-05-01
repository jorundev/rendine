#include <stdlib.h>
#include <png.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "texture.h"

char	*g_png_error_function = "?";
char	*g_png_error = "No Error";

const char *png_get_error(void)
{
	return g_png_error;
}

const char *png_get_error_function(void)
{
	return g_png_error_function;
}

void png_disp_error(struct png_struct_def *png, const char *err)
{
	printf("PNG Error: %s\n", err);
}

void png_disp_warn(struct png_struct_def *png, const char *warn)
{
	printf("PNG Warning: %s\n", warn);
}

bool texture_png_load_from_file(texture_t *tex, const char *filename)
{
	FILE			*fp = fopen(filename, "rb");
	unsigned char	sig[16] = {0};

	if (!fp) {
		g_png_error_function = "texture_png_load_from_file";
		g_png_error = strerror(errno);
		return false;
	}

	fread(sig, 1, 8, fp);
	if (png_sig_cmp(sig, 0, 7)) {
		g_png_error = "Not a PNG file";
		goto error;
	}

	png_structp png_ptr = png_create_read_struct
		(PNG_LIBPNG_VER_STRING, (png_voidp)g_png_error,
		png_disp_error, png_disp_warn);
	if (!png_ptr) {
		g_png_error = "Invalid PNG file";
		goto error;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr,
			(png_infopp)NULL, (png_infopp)NULL);
		g_png_error = "Invalid PNG file";
		goto error;
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
			(png_infopp)NULL);
		g_png_error = "Invalid PNG file";
		goto error;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
			&end_info);
		g_png_error = "Invalid PNG file";
		goto error;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	tex->color_type = png_get_color_type(png_ptr, info_ptr);
	tex->size.width = png_get_image_width(png_ptr, info_ptr);
	tex->size.height = png_get_image_height(png_ptr, info_ptr);
	tex->bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	if (tex->bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}

	if (tex->color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}

	if (tex->color_type == PNG_COLOR_TYPE_GRAY && tex->bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	switch (tex->color_type) {
		case PNG_COLOR_TYPE_RGB:
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_PALETTE:
			png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
			break ;
		default:
			break ;
	}

	switch (tex->color_type) {
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(png_ptr);
			break ;
		default:
			break ;
	}

	png_read_update_info(png_ptr, info_ptr);

	unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	tex->raw = malloc(sizeof(png_byte) * tex->size.height * rowbytes);
	if (!tex->raw) {
		g_png_error = "Not enough RAM";
		goto error;
	}

	png_bytepp row_pointers = (png_bytepp)malloc(tex->size.height * sizeof(png_bytep));
	if (!rowbytes) {
		g_png_error = "Could not get row pointers";
		goto error;
	}

	for (unsigned int i = 0; i < tex->size.height; ++i) {
		row_pointers[tex->size.height - 1 - i] = tex->raw + i + rowbytes;
	}

	png_read_image(png_ptr, row_pointers);

	int alpha;

	switch (tex->color_type) {
		case PNG_COLOR_TYPE_RGBA:
			alpha = GL_RGBA;
			break ;
		case PNG_COLOR_TYPE_RGB:
			alpha = GL_RGB;
			break ;
		default:
			g_png_error = "Unsupported color format";
			goto error;
			break ;
	}

	glGenTextures(1, &tex->gl_obj);
	glBindTexture(GL_TEXTURE_2D, tex->gl_obj);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		tex->size.width,
		tex->size.height,
		0, alpha, GL_UNSIGNED_BYTE, (GLvoid *)tex->raw);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	fclose(fp);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(row_pointers);
	return true;

	error:	fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			g_png_error_function = "texture_png_load_from_file";
			return false;
}
