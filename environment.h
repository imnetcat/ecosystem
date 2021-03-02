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

	enum class Success
	{
		fail,
		normal,
		good
	};
	static Success SuccessRule(EntityIterator);
protected:

	size_t shift_count;

	size_t entities_count;
	size_t max_generation;
	TerrainCell terrain[ENVIRONMENT_SIZE_Y][ENVIRONMENT_SIZE_X];
	std::array<Entity, ENVIRONMENT_SIZE_Y * ENVIRONMENT_SIZE_X> entities;

private:
	void EntityDie(EntityIterator);

	Position GetViewedPosition(view_side view, size_t x, size_t y);
	Position GetInvertedViewedPosition(view_side view, size_t x, size_t y);

	size_t Reproduction(EntityIterator parent_entity, EntityIterator new_entity);
	void Separationing(EntityIterator);
	void Birthing(EntityIterator);
	void Carnivorousing(EntityIterator);
	void Mineraling(EntityIterator);
	void Moving(EntityIterator);
	void Photosynthesing(EntityIterator);
	void Staying();
	void Turning(int args, EntityIterator);
};
