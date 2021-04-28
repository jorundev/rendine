#include "assets.h"

typedef
struct asset_s {
	asset_type_t	type;
	bool			reserved;
	void			*data;
	unsigned int	id;
}	asset_t;

typedef
struct texture_s {
}	texture_t;

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
