#pragma once
#include "genome.h"
#include "Object.h"
#include "config.h"
#include "ListPool.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <map>
#include <sstream>

enum class view_side
{
	top,
	right_top,
	right,
	right_bottom,
	bottom,
	left_bottom,
	left,
	left_top,
};

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

class Entity : public Object
{
protected:
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short max_hp;
	unsigned short energy;
	unsigned short max_energy;
	double defence;
	double attack;
	Genome genom;
	unsigned char carnivorousing = 0;
	unsigned char fotosintesis = 0;
	unsigned char mineraling = 0;
public:
	Entity();
	Entity(
		size_t x,
		size_t y,
		view_side view,
		unsigned short max_hp,
		unsigned short energy,
		unsigned short max_energy,
		unsigned short max_age,
		double defence,
		double attack,
		Genome g
	);


	bool IsDead();

	size_t ReproductionCost();

	const Genome& GetGenome() const;
	Genome& GetGenome();
	
	void Tic();

	bool Defencing(double);
	double Defence();
	double Attack();
	void AttackUp();
	unsigned short Age();
	unsigned short MaxAge();
	view_side GetView();
	void SetView(view_side val);
	void IncreaceEnergy(unsigned short value);
	void DecreaceEnergy(unsigned short value);
	void IncreaceHp(unsigned short value);
	void DecreaceHp(unsigned short value);
	unsigned short Energy();
	unsigned short Hp();

	void SetGenome(Genome value);
	void Defence(double);
	void Attack(double);
	void Age(unsigned short);
	void MaxAge(unsigned short);
	void View(view_side);
	void Energy(unsigned short);
	void Hp(unsigned short);
};

using EntitiesIterator = ListPool<Entity>::iterator;
