#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <SDL_ttf.h>

class ui {
private:
	int _mX, _mY;
	Uint32 _mouseClickState;

protected:
	const char* _txt;
	uint16_t _x, _y;
	SDL_Color _clr;
	uint16_t _size;
	uint16_t _decrementAmt;
	
public:
	uint16_t w, h;

	ui(const char* txt, uint16_t x, uint16_t y, uint16_t _h, uint16_t _w, uint16_t size, SDL_Color clr);

	bool handleInput(uint16_t decrementAmt);
	void showText(SDL_Renderer* renderer);

#ifdef TEXTURE_H
	friend class texture;
#endif
};

#endif 

