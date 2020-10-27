#pragma once
#include "entity.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

struct NewCellStat
{
	NewCellStat(Genome g) : genom(g) {}
	bool is_mutating;
	unsigned short hp;
	unsigned short energy;
	unsigned short max_age;
	double defence;
	unsigned short attack;
	Genome genom;
	size_t separation_cost;
	size_t birth_cost;
};

class Cell : public Entity
{
protected:
	Genome genom;
	const size_t separation_cost;
	const size_t birth_cost;
public:
	Cell(Cell&& obj) :
		Entity(std::move(obj.Hp()), std::move(obj.Energy()), std::move(obj.MaxAge()),
			std::move(obj.Defence()),
			std::move(obj.Attack())),
		genom(std::move(obj.GetGenome())),
		separation_cost(std::move(obj.SeparationCost())),
		birth_cost(std::move(obj.BirthCost())) {}

	explicit Cell(
		unsigned short energy,
		unsigned short max_age,
		size_t sepr_cost,
		size_t birthcost,
		double defence,
		unsigned short attack,
		Genome g)
		: 
		Entity(MAX_HP, energy, max_age, defence, attack),
		genom(g), separation_cost(sepr_cost), birth_cost(birthcost) {}
	
	size_t SeparationCost()
	{
		return separation_cost;
	}
	size_t BirthCost()
	{
		return birth_cost;
	}

	Genome GetGenome()
	{
		return genom;
	}
	
	bool IsFriendly(Cell* cell)
	{
		const unsigned short BORDER = 8;
		size_t count_of_non_equal = 0;
		auto lhs_genom = GetGenome().data;
		auto rhs_genom = cell->GetGenome().data;

		for (size_t index = 0; index < lhs_genom.size(); index++)
		{
			if (lhs_genom[index].Read() != rhs_genom[index].Read())
				count_of_non_equal++;

			if (count_of_non_equal > BORDER)
				return false;
		}

		return true;
	}

	Cell* Separation()
	{
		NewCellStat new_cell = Reproduction();

		DecreaceEnergy(separation_cost);
		unsigned short hlph = energy / 2;
		DecreaceEnergy(hlph);
		new_cell.energy += hlph;

		return Mutation(new_cell);
	}

	Cell* Birth()
	{
		return Mutation(Reproduction());
	}
	
	void Tic(std::vector<Gen::Command>& commands) override
	{
		if (!hp || age == max_age)
			commands.push_back(Gen::Command::die);
		else
			commands.push_back(genom.Read());
		
		age++;
	}


	RGBColor TerrainColor() override
	{
		return { 119, 255, 110 };
	}
	RGBColor MineralsColor() override
	{
		return { 209, 209, 209 };
	}
	RGBColor EnergyColor() override
	{
		if (energy < (MAX_ENERGY / 2))
		{
			return { 255, static_cast<unsigned char>(255 * (energy / (double)(MAX_ENERGY / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(255 - 255 * (energy / (double)MAX_ENERGY)), 255, 0 };
		}
	}
	RGBColor SpeciesColor() override
	{
		return Species();
	}
	RGBColor AgeColor() override
	{
		unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)age / max_age));
		return { c, c, c };
	}
	RGBColor HpColor() override
	{
		if (hp < (MAX_HP / 2))
		{
			return { 191, static_cast<unsigned char>(191 * (hp / (double)(MAX_HP / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(191 * ((double)(MAX_HP / 2) / hp)), 191, 0 };
		}
	}
	RGBColor SurvivalColor() override
	{
		double succes_survival = CellSuccessRule(energy, max_age, 1, -1);
		if (succes_survival == 0)
		{
			return { 255, 225, 0 };
		}
		else if (succes_survival == -1)
		{
			return { 255, 21, 0 };
		}
		else
		{
			return { 0, 194, 0 };
		}
	}


protected:

	RGBColor Species() override
	{
		return genom.Hash();
	}
	NewCellStat Reproduction()
	{
		srand(time(0) + rand());
		auto new_genom = genom.data;
		float mt = (rand() % 100) / (double)100;
		if (mt < genom.mutationChance)
		{
			size_t index = rand() % Genome::length;
			new_genom[index] = Gen();
		}

		
		short max_age_koef = CellSuccessRule(energy, max_age, 1, -1);
		unsigned short new_max_age = max_age + max_age_koef;
		if (new_max_age > Genome::length * 4) new_max_age = Genome::length * 4;
		if (new_max_age < Genome::length) new_max_age = Genome::length;
		
		float mutationChance_koef = CellSuccessRule(energy, max_age, -0.01, 0.01);
		float new_mutationChance = genom.mutationChance + mutationChance_koef;
		if (new_mutationChance > 1) new_mutationChance = 1;
		if (new_mutationChance < 0.01) new_mutationChance = 0.01;
		

		NewCellStat stat(Genome(new_genom, new_mutationChance, genom.generation + 1));
		stat.attack = attack;
		stat.defence = defence;
		stat.birth_cost = birth_cost;
		stat.separation_cost = separation_cost;
		stat.max_age = max_age;
		stat.energy = 100;
		stat.is_mutating = false;
			   
		if (mt < genom.mutationChance)
		{
			stat.is_mutating = true;
		}

		return stat;
	}
	virtual Cell* Mutation(NewCellStat) = 0;

	float CellSuccessRule(size_t accumulated_energy, size_t max_age, float success, float fail)
	{
		return (accumulated_energy > ((3 * MAX_ENERGY) / 4)) ? success :
			(accumulated_energy > (MAX_ENERGY / 4) ? 0 : fail);
	}
};
