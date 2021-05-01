#pragma once

#include "Asset.hpp"
#include "GLObject.hpp"
#include "utils/Result.hpp"
#include <png.h>

namespace rendine {

enum class TextureColorType {
	Unknown = -1,
	Grayscale = PNG_COLOR_TYPE_GRAY,
	GrayscaleAlpha = PNG_COLOR_TYPE_GRAY_ALPHA,
	RGB = PNG_COLOR_TYPE_RGB,
	RGBA = PNG_COLOR_TYPE_RGB_ALPHA,
	Palette = PNG_COLOR_TYPE_PALETTE,
};

class Texture : public Asset, public GLObject {
public:
	Texture()
		: Asset(),
		color_type(TextureColorType::Unknown),
		width(0), height(0),
		bit_depth(0)
	{};

	~Texture() override;

	virtual void	unload() override;

	Result<void, const char *>	loadFromFile(const char *filename);

private:
	TextureColorType	color_type;
	unsigned int		width, height, bit_depth;
	unsigned char		*raw;
};

}
