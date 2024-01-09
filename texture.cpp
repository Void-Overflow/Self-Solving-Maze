#include "texture.h"

texture::texture(SDL_Renderer* renderer, std::vector<std::tuple<ui, bool, uint16_t>>texts) {
	_renderer = renderer;
	for (int i = 0; i < texts.size(); i++) {
		_textures.push_back(texts.at(i));
		_initialDecrement.push_back(false);
	}
};

void texture::updateAllTextures() {
	SDL_RenderClear(_renderer);
	for (int i = 0; i < _textures.size(); i++) 
		std::get<0>(_textures.at(i)).showText(_renderer);
}

void texture::initialUpdate() {
	if (_initialRun == false) {
		updateAllTextures();
		_initialRun = true;
	}
}

int texture::checkInterfaces() {
	for (int i = 0; i < _textures.size(); i++) {
		if (std::get<1>(_textures.at(i)) == true) {
			if(std::get<0>(_textures.at(i)).handleInput(std::get<2>(_textures.at(i))))
				return i;

			if (std::get<0>(_textures.at(i))._decrementAmt != 0 && _initialDecrement.at(i) == false) {
				updateAllTextures();
				_initialDecrement.at(i) = true;
			}
			if (std::get<0>(_textures.at(i))._decrementAmt == 0 && _initialDecrement.at(i) == true) {
				updateAllTextures();
				_initialDecrement.at(i) = false;
			}
		}
	}
	return -1;
}

void texture::changeAttributes(uint16_t index, attributes property, uint16_t value, SDL_Color clr = { NULL, NULL, NULL })
{
	if (property == attributes::color) 
		std::get<0>(_textures.at(index))._clr = clr;
	else if(property == attributes::h)
		std::get<0>(_textures.at(index)).h = value;
	else if (property == attributes::size)
		std::get<0>(_textures.at(index))._size = value;
	else if (property == attributes::w)
		std::get<0>(_textures.at(index)).w = value;
	else if (property == attributes::x)
		std::get<0>(_textures.at(index))._x = value;
	else if (property == attributes::y)
		std::get<0>(_textures.at(index))._y = value;
	updateAllTextures();
}
