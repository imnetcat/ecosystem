#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
#include <list>
#include <cmath>
#include "config.h"
#include "TerrainCell.h"

#include "sfml.h"

using namespace std;

struct Position
{
	size_t x;
	size_t y;
};

bool operator == (const Position& lhs, const Position& rhs);

class Environment
{
public:
	explicit Environment();
	void Update();
protected:

	size_t cells_count;
	size_t max_generation;
	TerrainCell terrain[ENVIRONMENT_SIZE_Y][ENVIRONMENT_SIZE_X];
	std::list<Entity> entities;

private:
	void Shift(size_t i);
	void CellDie(size_t index, size_t x, size_t y);
	void CellDie(size_t x, size_t y);

	Position GetViewedPosition(view_side view, Position init);
	Position GetInvertedViewedPosition(view_side view, Position init);
private:
	void Separationing(size_t x, size_t y);
	void Birthing(size_t x, size_t y);
	void Carnivorousing(size_t x, size_t y);
	void Mineraling(size_t x, size_t y);
	void Moving(size_t x, size_t y);
	void Photosynthesing(size_t x, size_t y);
	void Staying();
	void Turning(int args, size_t x, size_t y);
};
