#ifndef ASTAR_PATHFINDING_H
#define ASTAR_PATHFINDING_H

#include "maze.h"
#include <tuple>

class nodes {
private:
	std::pair<uint16_t, uint16_t> _coordinate;
	std::vector<std::pair<uint16_t, uint16_t>> _obstacles;
	uint16_t _h, _w;
public:
	enum loc {RIGHT, LEFT, TOP, BOTTOM};

	nodes(std::pair<uint16_t, uint16_t> coordinate, std::vector<std::pair<uint16_t, uint16_t>> obstacles, uint16_t h, uint16_t w);

	std::array<std::pair<int16_t, int16_t>, 4> locateNodes(std::vector<std::pair<int16_t, int16_t>> closedSet); //only returns all viable nodes around a current position
}; 

class aStarPathFinding
{
private:
	maze _m;
	std::vector<std::pair<int16_t, int16_t>> _openSet;
	std::vector<std::pair<int16_t, int16_t>> _closedSet;

	enum nodeCosts {gCost, hCost, fCost};
	std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> _openSetCosts;

	std::pair<uint16_t, uint16_t> _target, _start;
	uint16_t _h, _w;
protected:
	std::vector<std::pair<int16_t, int16_t>> _reconstructedPath;
	void reconstructPath();
public:
	aStarPathFinding();
	aStarPathFinding(maze m, std::pair<uint16_t, uint16_t> start, std::pair<int16_t, int16_t> target = std::make_pair(-1, -1));

	void nextFeasableNode();
};

#endif