#pragma once

namespace rendine {

class Asset {
public:
	Asset() : is_loaded(false) {	};
	virtual ~Asset() = default;

	bool	isLoaded() { return this->is_loaded; };
	virtual void	unload() = 0;

protected:
	bool	is_loaded;
};

}
