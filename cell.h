#pragma once
#include "genome.h"

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
	minerals,
	ration,
	energy,
	species,
	age,
	hp,
	survival,
	generations
};

class Cell
{
protected:
	size_t x;
	size_t y;
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short energy;
	double defence;
	double attack;
	Genome genom;
	size_t reproduction_cost;
	unsigned char carnivorousing = 0;
	unsigned char fotosintesis = 0;
	unsigned char mineraling = 0;
public:
	Cell();
	Cell(
		size_t x,
		size_t y,
		unsigned short energy,
		unsigned short max_age,
		size_t repr_cost,
		double defence,
		double attack,
		Genome g
	);
	Cell(Cell&& cell);
	Cell(const Cell& cell);
	Cell& operator = (const Cell& cell);
	Cell& operator = (Cell&& cell);

	size_t GetX() const;
	size_t GetY() const;
	void SetPosition(size_t nx, size_t ny);

	bool IsDead();

	size_t ReprodutionCost();

	const Genome& GetGenome() const;
	Genome& GetGenome();
	
	bool IsFriendly(const Cell& cell);

	RGBColor RationColor();
	RGBColor Species();

	void Reproduction(Cell&);

	void Tic();

	void RationLevel(char r, char g, char b);

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
	void ReproductionCost(size_t);
	void Defence(double);
	void Attack(double);
	void Age(unsigned short);
	void MaxAge(unsigned short);
	void View(view_side);
	void Energy(unsigned short);
	void Hp(unsigned short);

	enum class Success
	{
		fail,
		normal,
		good
	};
	Success SuccessRule();
protected:
};
