#include "aStarPathFinding.h"

#include <array>
#include <algorithm>
#include <stdexcept>

nodes::nodes(std::pair<uint16_t, uint16_t> coordinate, std::vector<std::pair<uint16_t, uint16_t>> obstacles, uint16_t h, uint16_t w)
{
	_coordinate = coordinate;
	_obstacles = obstacles;
	_h = h;
	_w = w;
}

std::array<std::pair<int16_t, int16_t>, 4> nodes::locateNodes(std::vector<std::pair<int16_t, int16_t>> closedSet)
{
	std::array<std::pair<int16_t, int16_t>, 4> surroundingNodes;
	surroundingNodes[TOP] = std::make_pair(_coordinate.first, (_coordinate.second - 1 < 0) ? -1 : _coordinate.second - 1);
	surroundingNodes[BOTTOM] = std::make_pair(_coordinate.first, (_coordinate.second + 1 >= _h) ? -1 : _coordinate.second + 1);
	surroundingNodes[RIGHT] = std::make_pair((_coordinate.first + 1 >= _w) ? -1 : _coordinate.first + 1, _coordinate.second);
	surroundingNodes[LEFT] = std::make_pair((_coordinate.first + 1 < 0) ? -1 : _coordinate.first - 1, _coordinate.second);

	for (int i = 0; i < _obstacles.size(); i++) {
		for (int pos = 0; pos < 4; pos++) {
			if (surroundingNodes[pos] == _obstacles.at(i))
				surroundingNodes[pos] = std::make_pair(-1, -1);
		}
	}
	for (int i = 0; i < closedSet.size(); i++) {
		for (int pos = 0; pos < 4; pos++) {
			if (surroundingNodes[pos] == closedSet.at(i))
				surroundingNodes[pos] = std::make_pair(-1, -1);
		}
	}

	return surroundingNodes;
}

aStarPathFinding::aStarPathFinding() {}
aStarPathFinding::aStarPathFinding(maze m, std::pair<uint16_t, uint16_t> start, std::pair<int16_t, int16_t> target)
{
	_m = m;
	_target = (target.first == -1 || target.second == -1) ? _m._target : (std::pair<uint16_t, uint16_t>)target;
	_target.first--;
	_target.second--;
	_start = start;
	_h = _m._h/20;
	_w = _m._w/20;
	_closedSet.push_back(std::make_pair(_m._cursorPos.at(0).first, _m._cursorPos.at(0).second));
}

void aStarPathFinding::reconstructPath() {
	_reconstructedPath.clear();
	_reconstructedPath.push_back(_closedSet.at(_closedSet.size()-2));

	while (_reconstructedPath.back() != _start && _reconstructedPath.back().first > -1 && _reconstructedPath.back().second > -1) {
		nodes neighboringNodesObj(_reconstructedPath.back(), _m._obstaclesFilled, _h, _w);

		std::vector<std::pair<int16_t, int16_t>> tempSet;
		for (int i = _openSet.size() - 1; i >= 0; i--) {
			bool failFlag = false;
			for (int x = 0; x < _closedSet.size() - 1; x++) {
				if (_openSet.at(i) == _closedSet.at(x))
					failFlag = true;
			}
			if (failFlag == false)
				tempSet.push_back(_openSet.at(i));
		}

		std::array<std::pair<int16_t, int16_t>, 4> neighboringNodes = neighboringNodesObj.locateNodes(tempSet);
		uint16_t neighboringGCosts[4] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};

		for (int i = 0; i < 4; i++) {
			if (neighboringNodes[i].first > -1 && neighboringNodes[i].second > -1) {
				bool flag = false;
				for (int x = 0; x < _reconstructedPath.size(); x++) {
					if (neighboringNodes[i] == _reconstructedPath.at(x))
						flag = true;
				}

				if(flag == false)
					neighboringGCosts[i] = abs(_start.first - neighboringNodes[i].first) + abs(_start.second - neighboringNodes[i].second);
			}
		}

		uint16_t* smallestGCostResult = std::min_element(neighboringGCosts, neighboringGCosts + 3);
		int smallestGCostIndex = std::distance(neighboringGCosts, smallestGCostResult); //find node with smallest G cost
		_reconstructedPath.push_back(neighboringNodes[smallestGCostIndex]);
	}
}

void aStarPathFinding::nextFeasableNode()
{
	if (_closedSet.back() == _target)
	{
		if (_reconstructedPath.size() > 0)
			return;
		reconstructPath();
		for (int i = 0; i < _reconstructedPath.size(); i++) 
				_m.addPos(_reconstructedPath.at(i), { 0, 160, 250, 100 });
	}

	else {
		nodes neighboringNodesObj(_closedSet.back(), _m._obstaclesFilled, _h, _w);
		std::array<std::pair<int16_t, int16_t>, 4> neighboringNodes = neighboringNodesObj.locateNodes(_closedSet);

		uint16_t currentGCost = abs(_closedSet.back().first - _start.first) + abs(_closedSet.back().second - _start.second);
		
		for (int i = 0; i < 4; i++) { //identify all new neighboring nodes
			if (neighboringNodes[i].first > -1 && neighboringNodes[i].second > -1) {
				_openSet.push_back(neighboringNodes[i]);
				int latestIndex = _openSet.size() - 1;

				_openSetCosts.push_back(std::make_tuple( //gCost, hCost, fCost
					abs(_start.first - _openSet.at(latestIndex).first) + abs(_start.second - _openSet.at(latestIndex).second), //manhattan distance from node to starting position (gCost)
					abs(_openSet.at(latestIndex).first - _target.first) + abs(_openSet.at(latestIndex).second - _target.second), //manhattan distance from node to final position (hCost)
					abs(_start.first - _openSet.at(latestIndex).first) + abs(_start.second - _openSet.at(latestIndex).second) //sum of g and h cost	(fCost)
					+ abs(_openSet.at(latestIndex).first - _target.first) + abs(_openSet.at(latestIndex).second - _target.second)
				));
			}
		}

		std::vector<uint16_t> fCosts;
		for (int i = 0; i < _openSetCosts.size(); i++)
			fCosts.push_back(std::get<fCost>(_openSetCosts.at(i)));
		std::vector<uint16_t>::iterator smallestFCostResult = std::min_element(fCosts.begin(), fCosts.end());
		uint8_t smallestFCostIndex = std::distance(fCosts.begin(), smallestFCostResult); //find node with smallest F cost

		std::vector<uint16_t>smallestFCostsIndicies;
		smallestFCostsIndicies.push_back(smallestFCostIndex);

		for (int i = 0; i < _openSetCosts.size(); i++) {
			if (std::get<fCost>(_openSetCosts.at(smallestFCostIndex)) == std::get<fCost>(_openSetCosts.at(i)) && i != smallestFCostIndex)
				smallestFCostsIndicies.push_back(i);
		}
		for (int i = 0; i < smallestFCostsIndicies.size(); i++) {
			try {
				if (std::get<hCost>(_openSetCosts.at(i)) < std::get<hCost>(_openSetCosts.at(smallestFCostIndex))) //if multiple with same smallest f cost, base off h cost
					smallestFCostIndex = smallestFCostsIndicies.at(i);
			}
			catch (const std::out_of_range& e) { std::exit(EXIT_FAILURE); }
		}

		_closedSet.push_back(std::make_pair(_openSet.at(smallestFCostIndex).first, _openSet.at(smallestFCostIndex).second));
		_openSet.erase(_openSet.begin() + smallestFCostIndex);
		_openSetCosts.erase(_openSetCosts.begin() + smallestFCostIndex);

		for (int i = 0; i < _openSet.size(); i++)
			_m.addPos(_openSet.at(i), { 0, 255, 25, 100 });
		for (int i = 0; i < _closedSet.size(); i++)
			_m.addPos(_closedSet.at(i), { 255, 0, 0, 100 });
	}
}