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
	explicit Environment(
		unsigned int width, 
		unsigned int height,
		unsigned short light_power,
		double light_coef,
		unsigned short max_orginic_to_eat,
		unsigned short max_entities_to_eat,
		unsigned short max_energy,
		unsigned short max_hp
	); 
	~Environment();

	void Update();

	Coefficient SuccessRule(EntitiesIterator);
protected:

	const unsigned int width;
	const unsigned int height;

	const unsigned short light_power;
	const double light_coef;

	const unsigned short max_organic_to_eat;
	const unsigned short max_entities_to_eat;
	const unsigned short max_energy;
	const unsigned short max_hp;

	size_t max_generation;
	TerrainCell** terrain;
	ListPool<Entity> entities;
	ListPool<Organic> organic;

	Entity* observed_entity = nullptr;

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
