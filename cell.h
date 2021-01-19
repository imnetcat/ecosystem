#pragma once
#include "genome.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <map>
#include <sstream>

class Cell : public Object
{
protected:
	size_t x;
	size_t y;
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short energy;
	float defence;
	unsigned short attack;
	Genome genom;
	size_t separation_cost;
	size_t birth_cost;
	std::unordered_map<Protein, unsigned long> proteins;
	std::vector<Trigger> organelles;
public:
	Cell();
	Cell(
		size_t x,
		size_t y,
		unsigned short energy,
		unsigned short max_age,
		size_t sepr_cost,
		size_t birthcost,
		float defence,
		unsigned short attack,
		Genome g,
		const std::vector<Trigger> organelles
	);
	Cell(Cell&& cell);
	Cell(const Cell& cell);
	Cell& operator = (const Cell& cell);
	Cell& operator = (Cell&& cell);

	size_t GetX() const;
	size_t GetY() const;
	void SetPosition(size_t nx, size_t ny);

	bool IsDead();

	size_t SeparationCost();
	size_t BirthCost();

	const Genome& GetGenome() const;
	
	bool IsFriendly(const Cell& cell);

	void Separation(Cell&);
	void Birth(Cell&);

	std::unordered_map<Protein, unsigned long>& Proteins();
	const std::unordered_map<Protein, unsigned long>& Proteins() const;

	const std::vector<Trigger>& Organelles();

	RGBColor TerrainColor() override;
	RGBColor MineralsColor() override;
	RGBColor EnergyColor() override;
	RGBColor SpeciesColor() override;
	RGBColor AgeColor() override;
	RGBColor HpColor() override;
	RGBColor SurvivalColor() override;

	void Tic();

	RGBColor RationColor() override;

	bool Outline(view_settings) override;
	bool Defenced(unsigned short);
	float Defence();
	unsigned short Attack();
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
	void Organelles(std::vector<Trigger> value);
	void SeparationCost(size_t);
	void BirthCost(size_t);
	void Defence(float);
	void Attack(unsigned short);
	void Age(unsigned short);
	void MaxAge(unsigned short);
	void View(view_side);
	void Energy(unsigned short);
	void Hp(unsigned short);
protected:

	RGBColor Species();
	void Reproduction(Cell&);

	enum class CellSuccess
	{
		fail,
		normal,
		good
	};

	CellSuccess CellSuccessRule(size_t accumulated_energy, unsigned short age, unsigned short max_age);
};
