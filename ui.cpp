#include "ui.h"

ui::ui(const char* txt, uint16_t x, uint16_t y, uint16_t _h, uint16_t _w, uint16_t size, SDL_Color clr) {
	_x = x;
	_y = y;
	h = _h;
	w = _w;
	_txt = txt;
	_size = size;
	_clr = clr;
	_decrementAmt = 0;
}

bool ui::handleInput(uint16_t decrementAmt) {
	SDL_PumpEvents();
	_mouseClickState = SDL_GetMouseState(&_mX, &_mY);

	if ((_y  <= _mY && _mY <= h + _y ) && (_x <= _mX && _mX <= w + _x )) { //checks if mouse is within domain and range of box
		_decrementAmt = decrementAmt;

		if ((_mouseClickState & SDL_BUTTON_LMASK) != 0)
			return true;
	}
	else
		_decrementAmt = 0;
	return false;
}

void ui::showText(SDL_Renderer* renderer){
	TTF_Font* font = TTF_OpenFont("OpenSans-Bold.ttf", _size);
	SDL_Surface* SurfaceMessage =  TTF_RenderText_Solid(font, _txt, _clr);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, SurfaceMessage);
	SDL_Rect Message_rect;

	Message_rect.x = _x;
	Message_rect.y = _y;
	Message_rect.w = w - _decrementAmt;
	Message_rect.h = h;

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	SDL_DestroyTexture(Message);
	SDL_FreeSurface(SurfaceMessage);
	TTF_CloseFont(font);
}
