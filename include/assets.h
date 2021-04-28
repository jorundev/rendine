#pragma once

#include <stdlib.h>
#include "types.h"

#define MAX_ASSET_NUMBER 1024

typedef
/*!
 *	\brief Represents the type of an asset.
 */
enum asset_type_e {
	ASSET_TYPE_NONE,
	ASSET_TYPE_TEXTURE,
	ASSET_TYPE_MODEL,
	ASSET_TYPE_SHADER,
	ASSET_TYPE_MATERIAL,
	ASSET_TYPE_OTHER,
}	asset_type_t;

/*!
 *	\brief A handle to an asset. Hides any implementation details.
 *         Interact with it using the asset_*() functions.
 */
typedef struct asset_s *assethdnl_t;

/*! \brief Creates a new asset with type 'type'
 *
 *  \param type type of the asset
 *  \return The asset handle.
 */
assethdnl_t asset_new(asset_type_t type);

/*! \brief Determines if the asset uses Progressive Quality Loading.
 *
 * Progressive Quality Loading (PQL) is the process of loading an asset at
 * varying 'qualities'. For example, this can be used to have multiple texture
 * resolution, and using a low-resolution version of the texture when the
 * object if far away, or when the full-resolution texture is not yet
 * fully loaded (when streaming assets).
 *
 *  \param asset an asset handle
 *  \return true if the asset uses progressive quality loading, false otherwise.
 */
bool asset_uses_prog_quality_loading(assethdnl_t asset);

/*! \brief Returns the currently loaded quality
 *
 *  \param asset an asset handle
 *  \return The currently loaded quality level. -1 if no qualities are loaded. -2 if the
 *  asset doesn't use quality loading
 */
int asset_current_loaded_quality(assethdnl_t asset);

/*! \brief The highest quality than can be currently loaded on this asset.
 *
 *  \param asset an asset handle
 *  \return The maximum loadable quality level. -1 if no qualities are loaded. -2 if the
 *  asset doesn't use quality loading
 */
int asset_max_loadable_quality(assethdnl_t asset);

/*! \brief The total number of qualities on this asset.
 *
 *  \param asset an asset handle
 *  \return The total number of qualities on this asset.
 */
int asset_num_quality(assethdnl_t asset);

/*! \brief Determines is the asset is loaded into memory.
 *         Note: If the asset uses Progressive Quality Loading (see asset_uses_prog_quality_loading),
 *         this will evaluate to true if any quality is loaded.
 *
 *  \param asset an asset handle
 *  \return true if the asset is loaded, false otherwise.
 */
bool asset_is_loaded(assethdnl_t asset);

/*! \brief Loads the asset from a file. The asset has to have a type.
 *
 *  \param asset an asset handle
 *  \param filename path to the asset on the filesystem
 *  \return true if the operation executed successfully, false otherwise.
 *          Use asset_get_error() to get more information in case of failure.
 */
bool asset_load_from_file(assethdnl_t asset, const char *filename);

/*! \brief Releases the memory of the asset.
 *
 *  \param asset an asset handle
 */
void asset_release(assethdnl_t asset);


/*! \brief Releases all assets
 *
 */
void asset_release_all(void);

/*! \brief Sets the type of the asset. Releases the asset beforehand.
 *
 *  \param asset an asset handle
 *  \param type type of the asset
 */
void asset_set_type(assethdnl_t asset, asset_type_t type);
