#ifndef MAZE_H
#define MAZE_H

#include <SDL.h>
#include <vector>


class maze
{
private:
	SDL_Renderer* _renderer;
	bool _initialFlag = false;

	uint16_t _w, _h;
	std::pair<uint16_t, uint16_t> _target;

	std::vector<std::pair<uint16_t, uint16_t>> _obstaclesFilled;
	std::vector<std::pair<uint16_t, uint16_t>> _cursorPos;

	void _generateGrid();
	void _displayMaze();

	bool _checkMovementConflict(std::pair<int16_t, int16_t > c);
protected:
	void addPos(std::pair<uint16_t, uint16_t> c, SDL_Color clr = { 25, 250, 25, 100 });
public:
	enum difficulty {easy, medium, hard};
	int abcd;
	maze();
	maze(SDL_Renderer* renderer, uint16_t h, uint16_t w);

	bool generateRandomMaze(difficulty dif, std::pair<uint16_t, uint16_t> target, bool hardReset = false);
	void moveUsrCursor(int16_t x, int16_t y);  

#ifdef ASTAR_PATHFINDING_H
	friend class aStarPathFinding;
#endif
};

#endif 
