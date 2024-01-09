#ifndef TEXTURE_H
#define TEXTURE_H

#include "ui.h"

#include <vector>
#include <tuple>

class texture
{
private:
	SDL_Renderer* _renderer;
	std::vector<std::tuple<ui, bool, uint16_t>> _textures;

	bool _initialRun = false;
	std::vector<bool>_initialDecrement;


public:
	enum attributes { x, y, h, w, size, color };

	texture(SDL_Renderer* renderer, std::vector<std::tuple<ui, bool, uint16_t>>texts);

	void updateAllTextures();
	void initialUpdate();

	int checkInterfaces();
	void changeAttributes(uint16_t index, attributes property, uint16_t value, SDL_Color clr);
};

#endif 

