#include <rendine/Texture.hpp>

namespace rendine {

Texture::~Texture()
{
	this->unload();
}

void Texture::unload()
{
	this->is_loaded = false;
}

static Result<void, const char *> check_png_signature(unsigned char *signature)
{
	if (png_sig_cmp(signature, 0, 7)) {
		return Err("Not a PNG file");
	}
	return (Ok());
}

void png_disp_error(struct png_struct_def *png, const char *err)
{
	printf("PNG Error: %s\n", err);
}

void png_disp_warn(struct png_struct_def *png, const char *warn)
{
	printf("PNG Warning: %s\n", warn);
}

Result<void, const char *> Texture::loadFromFile(const char *filename)
{
	FILE			*fp = fopen(filename, "rb");
	unsigned char	sig[16] = {0};

	if (!fp) {
		return Err( (const char *)strerror(errno) );
	}

	fread(sig, 1, 8, fp);
	if (check_png_signature(sig).isErr()) {
		fclose(fp);
		return Err( "Not a PNG file" );
	}

	png_structp png_ptr = png_create_read_struct(	PNG_LIBPNG_VER_STRING,
													nullptr,
													png_disp_error,
													png_disp_warn);

	if (png_ptr == nullptr) {
		fclose(fp);
		return Err( "Could not create PNG struct" );
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return Err( "Invalid PNG file" );
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (end_info == nullptr)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return Err( "Invalid PNG file" );
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return Err( "Invalid PNG file" );
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	this->color_type = (TextureColorType)png_get_color_type(png_ptr, info_ptr);
	this->width = png_get_image_width(png_ptr, info_ptr);
	this->height = png_get_image_height(png_ptr, info_ptr);
	this->bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	if (this->bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}

	if (this->color_type == TextureColorType::Palette) {
		png_set_palette_to_rgb(png_ptr);
	}

	if (this->color_type == TextureColorType::Grayscale && this->bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	switch (this->color_type) {
		case TextureColorType::RGB:
		case TextureColorType::Grayscale:
		case TextureColorType::Palette:
			png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
			break ;
		default:
			break ;
	}

	switch (this->color_type) {
		case TextureColorType::Grayscale:
		case TextureColorType::GrayscaleAlpha:
			png_set_gray_to_rgb(png_ptr);
			break ;
		default:
			break ;
	}

	png_read_update_info(png_ptr, info_ptr);

	unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	this->raw = (unsigned char *)malloc(sizeof(png_byte) * this->height * rowbytes);
	if (!this->raw) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return Err( "Not enough memory" );
	}

	png_bytepp row_pointers = (png_bytepp)malloc(this->height * sizeof(png_bytep));
	if (!rowbytes) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return Err( "Not enough RAM" );
	}

	for (unsigned int i = 0; i < this->height; ++i) {
		row_pointers[this->height - 1 - i] = this->raw + i + rowbytes;
	}

	png_read_image(png_ptr, row_pointers);

	int alpha;

	switch (this->color_type) {
		case TextureColorType::RGBA:
			alpha = GL_RGBA;
			break ;
		case TextureColorType::RGB:
			alpha = GL_RGB;
			break ;
		default:
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			return Err( "Unsupported color format" );
			break ;
	}

	glGenTextures(1, this->getHandlePtr());
	glBindTexture(GL_TEXTURE_2D, this->getHandle());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		this->width,
		this->height,
		0, alpha, GL_UNSIGNED_BYTE, (GLvoid *)this->raw);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	fclose(fp);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(row_pointers);
	return Ok();
}

}
