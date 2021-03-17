#pragma once
#include "cell.h"

enum class view_settings
{
	terrain,
	organic,
	ration,
	energy,
	species,
	age,
	hp,
	success,
	generations
};

class World
{
public:
	explicit World(
		unsigned int width, 
		unsigned int height,
		unsigned short light_power,
		double light_coef,
		unsigned short max_orginic_to_eat,
		unsigned short max_entities_to_eat,
		unsigned short max_energy,
		unsigned short max_hp
	); 
	~World();

	void Update();

	Coefficient SuccessRule(pool<Entity>::const_iterator) const;

	unsigned short MaxHp() const
	{
		return max_hp;
	}
	size_t MaxGeneration() const
	{
		return max_generation;
	}
	cell** const Terrain() const
	{
		return terrain;
	}
	const pool<Entity>& Entities() const
	{
		return entities;
	}
	const pool<Organic>& Organics() const
	{
		return organic;
	}

	unsigned int Width() const
	{
		return width;
	}
	unsigned int Height() const
	{
		return height;
	}

	unsigned int LightPower() const
	{
		return light_power;
	}
	double LightCoef() const
	{
		return light_coef;
	}
protected:

	const unsigned int width;
	const unsigned int height;

	const unsigned short light_power;
	const double light_coef;

	const unsigned short max_organic_to_eat;
	const unsigned short max_entities_to_eat;
	const unsigned short max_hp;

	size_t max_generation;
	cell** terrain;
	pool<Entity> entities;
	pool<Organic> organic;

private:

	EntitiesIterator EntityDie(EntitiesIterator);

	bool GetViewPos(unsigned __int8 arg, position& pos);

	EntitiesIterator Reproduction(EntitiesIterator parent_entity, const position& pos);
	void Separationing(unsigned __int8 args, EntitiesIterator);
	void Birthing(unsigned __int8 args, EntitiesIterator);
	void Carnivorousing(unsigned __int8 args, EntitiesIterator);
	void EatOrganic(EntitiesIterator);
	void Moving(unsigned __int8 args, EntitiesIterator);
	void Photosynthesing(EntitiesIterator);
};
