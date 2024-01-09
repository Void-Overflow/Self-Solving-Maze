#include "maze.h"

#include <random>

maze::maze() { srand((unsigned int)time(NULL)); }
maze::maze(SDL_Renderer* renderer, uint16_t h, uint16_t w)
{
	_w = w;
	_h = h;
	_renderer = renderer;

	srand((unsigned int)time(NULL));
}

void maze::_generateGrid()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 100);
	for (int x = _w / 20 - 20; x <= _w; x += 20) {
		SDL_Rect* rectangle = new SDL_Rect{ x, 0, 1, _h }; //x, y, w, l
		SDL_RenderFillRect(_renderer, rectangle);
	}
	for (int y = 0; y <= _h; y += 20) {
		SDL_Rect* rectangle = new SDL_Rect{ 0, y, _w, 1 };
		SDL_RenderFillRect(_renderer, rectangle); 
	}
}

bool maze::generateRandomMaze(difficulty dif, std::pair<uint16_t, uint16_t> target, bool hardReset)
{
	if (_initialFlag == false || hardReset == true) {
		SDL_RenderClear(_renderer);
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 100);
		_generateGrid();
		for (int i = 0; i < ((dif == easy) ? 350 : ((dif == medium) ? 500 : 600)); i++) {
			uint16_t* x = new uint16_t(rand() % (_w/20)), *y = new uint16_t(rand() % ((*x == (_w/20 - 1)) ? (_h/20 - 2) : (_h/20)) + ((*x == 0) ? 1 : 0));
			_obstaclesFilled.push_back(std::make_pair(*x, *y));
			delete x, y;
		}
		_displayMaze();

		_cursorPos.push_back(std::make_pair (0, 0));
		SDL_Rect* usrRectangle = new SDL_Rect{ 1, 1, 19, 19 };
		SDL_SetRenderDrawColor(_renderer, 0, 160, 250, 100);
		SDL_RenderFillRect(_renderer, usrRectangle);

		_target = target;
		SDL_Rect* targetRectangle = new SDL_Rect{ _target.first * 20 - 19, _target.second * 20 - 19, 19, 19 };
		SDL_SetRenderDrawColor(_renderer, 128, 0, 128, 100);
		SDL_RenderFillRect(_renderer, targetRectangle);
		
		_initialFlag = true;
		return true;
	}
	return false;
}

void maze::moveUsrCursor(int16_t x, int16_t y)
{
	SDL_RenderClear(_renderer);
	SDL_SetRenderDrawColor(_renderer, 0, 160, 250, 100);
	if (x == -1 && y == -1) {
		if(_cursorPos.size() > 1)
		_cursorPos.pop_back();
	}
	else {
		int16_t _cX = _cursorPos.at(_cursorPos.size() - 1).first, _cY = _cursorPos.at(_cursorPos.size() - 1).second;
		if (x == 1 && _checkMovementConflict(std::make_pair(_cX + 1, _cY)) == false)
			_cursorPos.push_back(std::make_pair(_cX + 1, _cY));
		else if (x == -1 && _checkMovementConflict(std::make_pair(_cX - 1, _cY)) == false)
			_cursorPos.push_back(std::make_pair(_cX - 1, _cY));
		if (y == 1 && _checkMovementConflict(std::make_pair(_cX, _cY - 1)) == false)
			_cursorPos.push_back(std::make_pair(_cX, _cY - 1));
		else if (y == -1 && _checkMovementConflict(std::make_pair(_cX, _cY + 1)) == false)
			_cursorPos.push_back(std::make_pair(_cX, _cY + 1));
	}

	for (int i = 0; i < _cursorPos.size(); i++) {
		SDL_Rect* rectangle = new SDL_Rect{ _cursorPos.at(i).first * 20 + 1, _cursorPos.at(i).second * 20 + 1, 19, 19 };
		SDL_RenderFillRect(_renderer, rectangle);
	}

	SDL_Rect* targetRectangle = new SDL_Rect{ _target.first * 20 - 19, _target.second * 20 - 19, 19, 19 };
	SDL_SetRenderDrawColor(_renderer, 128, 0, 128, 100);
	SDL_RenderFillRect(_renderer, targetRectangle);

	_generateGrid();
	_displayMaze();
}

void maze::_displayMaze()
{
	for (int i = 0; i < _obstaclesFilled.size(); i++) {
		SDL_Rect* rectangle = new SDL_Rect{ _obstaclesFilled.at(i).first * 20 + 1, _obstaclesFilled.at(i).second * 20 + 1, 19, 19 };
		SDL_RenderFillRect(_renderer, rectangle);
	}
}

bool maze::_checkMovementConflict(std::pair<int16_t, int16_t > c)
{
	for (int i = 0; i < _obstaclesFilled.size(); i++) {
		if (_obstaclesFilled.at(i).first == c.first && _obstaclesFilled.at(i).second == c.second)
			return true;
	}
	for (int i = 0; i < _cursorPos.size(); i++) {
		if (_cursorPos.at(i).first == c.first && _cursorPos.at(i).second == c.second)
			return true;
	}
	if (c.first < 0 || c.first >= _w)
		return true;
	if (c.second < 0 || c.second >= _h)
		return true;
	return false;
}

void maze::addPos(std::pair<uint16_t, uint16_t> c, SDL_Color clr)
{
	SDL_Rect* rectangle = new SDL_Rect{ c.first * 20 + 1, c.second * 20 + 1, 19, 19 };
	SDL_SetRenderDrawColor(_renderer, clr.a, clr.g, clr.b, clr.g);
	SDL_RenderFillRect(_renderer, rectangle);

	SDL_Rect* usrRectangle = new SDL_Rect{ 1, 1, 19, 19 };
	SDL_SetRenderDrawColor(_renderer, 0, 160, 250, 100);
	SDL_RenderFillRect(_renderer, usrRectangle);

	SDL_Rect* targetRectangle = new SDL_Rect{ _target.first * 20 - 19, _target.second * 20 - 19, 19, 19 };
	SDL_SetRenderDrawColor(_renderer, 128, 0, 128, 100);
	SDL_RenderFillRect(_renderer, targetRectangle);
}
