#include <stdio.h>
#include <string.h>
#include "assets.h"
#include "texture.h"
#include "shader.h"

typedef
struct asset_s {
	asset_type_t	type;
	bool			reserved;
	void			*data;
	unsigned int	id;
}	asset_t;

const char	*g_asset_error_function = "No Error";
const char	*g_asset_error = "No Error";

const char *asset_get_error(void)
{
	return g_asset_error;
}

const char *asset_get_error_function(void)
{
	return g_asset_error_function;
}

asset_t			asset_pool[MAX_ASSET_NUMBER];
unsigned int	num_assets = 0;
unsigned int	last_used_asset = 0;
unsigned int	first_free_asset = 0;

assethdnl_t asset_new(asset_type_t type)
{
	assethdnl_t ret = &asset_pool[first_free_asset];
	ret->id = first_free_asset++;
	if (first_free_asset >= last_used_asset) {
		last_used_asset = first_free_asset;
		++num_assets;
	}

	ret->reserved = true;
	ret->type = type;
	switch (type) {
		case ASSET_TYPE_TEXTURE:
			ret->data = malloc(sizeof(texture_t));
			memset(ret->data, 0, sizeof(texture_t));
			break ;
		case ASSET_TYPE_SHADER:
			ret->data = malloc(sizeof(shader_t));
			memset(ret->data, 0, sizeof(shader_t));
			break ;
		default:
			ret->data = NULL;
			break ;
	}
	return ret;
}

void asset_set_type(assethdnl_t asset, asset_type_t type)
{
	asset->type = type;
}

void asset_release(assethdnl_t asset)
{
	if (asset->data) {
		switch (asset->type) {
			case ASSET_TYPE_TEXTURE:
				free(((texture_t *)asset->data)->raw);
				glDeleteTextures(1, &((texture_t *)asset->data)->gl_obj);
				break ;
			case ASSET_TYPE_SHADER:
				free((void *)((shader_t *)asset->data)->shader_source);
				for (int i = 0; i < ((shader_t *)asset->data)->num_uniforms; ++i) {
					free(((shader_t *)asset->data)->uniform_names[i]);
				}
				free(((shader_t *)asset->data)->uniform_names);
				free(((shader_t *)asset->data)->uniform_types);
				break ;
			default:
				break ;
		}
	}
	free(asset->data);
	asset->reserved = false;
	if (asset->id > 0 && asset->id == last_used_asset) {
		--last_used_asset;
	}
	if (asset->id > 0 && asset->id < first_free_asset) {
		first_free_asset = asset->id;
	}
}

bool asset_load_from_file(assethdnl_t asset, const char *filename)
{
	switch (asset->type) {
		case ASSET_TYPE_TEXTURE:
			if (!texture_png_load_from_file((texture_t *)asset->data, filename)) {
				g_asset_error = png_get_error();
				g_asset_error_function = png_get_error_function();
				return false;
			}
			break ;
		case ASSET_TYPE_SHADER:
			if (!shader_load_from_spirv_file((shader_t *)asset->data, filename)) {
				g_asset_error = shader_get_error();
				g_asset_error_function = shader_get_error_function();
				return false;
			}
			break ;
		default:
			break ;
	}
	return true;
}

void asset_release_all(void)
{
	for (int i = 0; i < last_used_asset; ++i) {
		asset_release(&asset_pool[i]);
	}
}

const char *asset_shader_get_glsl_source(assethdnl_t asset)
{
	if (asset->type == ASSET_TYPE_SHADER) {
		return ((shader_t *)asset->data)->shader_source;
	}
	g_asset_error = "Asset is not a shader";
	g_asset_error_function = "asset_shader_get_glsl_source";
	return 0;
}
