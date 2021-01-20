#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
#include <cmath>
#include "water.h"

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

	static size_t cells_count;
	static size_t max_generation;
	static std::array<std::array<Structure, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
	static std::array<Cell*, ENVIRONMENT_SIZE_X* ENVIRONMENT_SIZE_Y> cells;

private:
	static void Shift(size_t i);
	static void CellDie(size_t index, size_t x, size_t y);
	static void CellDie(size_t x, size_t y);

	Position GetViewedPosition(view_side view, Position init);
	Position GetInvertedViewedPosition(view_side view, Position init);
private:
	void Separationing(int args, size_t x, size_t y);
	void Birthing(int args, size_t x, size_t y);
	void Carnivorousing(int args, size_t x, size_t y);
	void Mineraling(int args, size_t x, size_t y);
	void Moving(int args, size_t x, size_t y);
	void Photosynthesing(int args, size_t x, size_t y);
	void Staying(int args, size_t x, size_t y);
	void Turning(int args, size_t x, size_t y);
};
