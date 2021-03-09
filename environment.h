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

	static Coefficient SuccessRule(EntitiesIterator);
protected:

	size_t max_generation;
	TerrainCell terrain[ENVIRONMENT_SIZE_Y][ENVIRONMENT_SIZE_X];
	ListPool<Entity, ENVIRONMENT_SIZE_Y* ENVIRONMENT_SIZE_X> entities;
	ListPool<Organic, ENVIRONMENT_SIZE_Y* ENVIRONMENT_SIZE_X> organic;

private:
	EntitiesIterator EntityDie(EntitiesIterator);

	Position GetViewedPosition(view_side view, size_t x, size_t y);

	view_side GetViewSide(unsigned __int8 arg);

	EntitiesIterator Reproduction(EntitiesIterator parent_entity, size_t x, size_t y, view_side view);
	void Separationing(unsigned __int8 args, EntitiesIterator);
	void Birthing(unsigned __int8 args, EntitiesIterator);
	void Carnivorousing(EntitiesIterator);
	void EatOrganic(EntitiesIterator);
	void Moving(EntitiesIterator);
	void Photosynthesing(EntitiesIterator);
	void Turning(unsigned __int8 args, EntitiesIterator);
};
