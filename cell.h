#pragma once
#include "entity.h"
#include "genome.h"
#include "organelle.h"

// organelles
#include "separationing.h"
#include "carnivorousing.h"
#include "mineraling.h"
#include "birthing.h"
#include "photosynthesing.h"
#include "moving.h"
#include "sleeping.h"
#include "staying.h"
#include "turning.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <map>
#include <sstream>

const std::vector<Organelle*> ORGANELLES = {
	new Staying,
	new Sleeping,

	new Birthing,
	new Separationing,

	new Photosynthesing,
	new Carnivorousing,
	new Mineraling,

	new Moving,
	new Turning
};

class Cell : public Entity
{
protected:
	Genome genom;
	const size_t separation_cost;
	const size_t birth_cost;
	std::unordered_map<Protein, unsigned long> proteins;
	std::vector<Organelle*> organelles;
public:
	explicit Cell(
		unsigned short energy,
		unsigned short max_age,
		size_t sepr_cost,
		size_t birthcost,
		float defence,
		unsigned short attack,
		Genome g,
		const std::vector<Organelle*> organelles)
		: 
		Entity(MAX_HP, energy, max_age, defence, attack),
		genom(g), 
		separation_cost(sepr_cost),
		birth_cost(birthcost),
		organelles(organelles) 
	{
		auto size = static_cast<unsigned int>(Protein::Count);
		proteins.reserve(size);
		for (unsigned int i = 0; i < size; i++)
		{
			proteins[static_cast<Protein>(i % size)] = 0;
		}
	}

	bool IsDead() override
	{
		return !hp || age == max_age;
	}

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

		if (lhs_genom.size() != rhs_genom.size())
			return false;

		for (size_t index = 0; index < lhs_genom.size(); index++)
		{
			if (lhs_genom[index] != rhs_genom[index])
				count_of_non_equal++;

			if (count_of_non_equal > BORDER)
				return false;
		}

		return true;
	}

	Cell* Separation()
	{
		Cell* new_cell = Reproduction();

		DecreaceEnergy(separation_cost);
		unsigned short hlph = energy / 2;
		DecreaceEnergy(hlph);
		new_cell->IncreaceEnergy(hlph);

		return new_cell;
	}

	Cell* Birth()
	{
		Cell* new_cell = Reproduction();

		DecreaceEnergy(birth_cost);

		return new_cell;
	}

	std::unordered_map<Protein, unsigned long>& Proteins()
	{
		return proteins;
	}
	const std::unordered_map<Protein, unsigned long>& Proteins() const
	{
		return proteins;
	}

	const std::vector<Organelle*>& Organelles()
	{
		return organelles;
	}

	RGBColor TerrainColor() override
	{
		return { 13, 168, 19 };
	}
	RGBColor MineralsColor() override
	{
		return { 209, 209, 209 };
	}
	RGBColor EnergyColor() override
	{
		if (energy < (MAX_ENERGY / 2))
		{
			return { 255, static_cast<unsigned char>(255 * (energy / (float)(MAX_ENERGY / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(255 - 255 * (energy / (float)MAX_ENERGY)), 255, 0 };
		}
	}
	RGBColor SpeciesColor() override
	{
		return Species();
	}
	RGBColor AgeColor() override
	{
		unsigned char c = static_cast<unsigned char>(255 - 255 * ((float)age / max_age));
		return { c, c, c };
	}
	RGBColor HpColor() override
	{
		if (hp < (MAX_HP / 2))
		{
			return { 191, static_cast<unsigned char>(191 * (hp / (float)(MAX_HP / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(191 * ((float)(MAX_HP / 2) / hp)), 191, 0 };
		}
	}
	RGBColor SurvivalColor() override
	{
		switch (CellSuccessRule(energy, age, max_age))
		{
		case Cell::CellSuccess::fail:
			return { 255, 21, 0 };
			break;
		case Cell::CellSuccess::normal:
			return { 255, 225, 0 };
			break;
		case Cell::CellSuccess::good:
			return { 0, 194, 0 };
			break;
		default:
			return { 255, 225, 0 };
			break;
		}
	}

	void Tic(MapTerrain& terrain, size_t& x, size_t& y) override
	{
		auto p = genom.Read();
		if (!proteins.count(p))
		{
			proteins[p] = 1;
		}
		else
		{
			proteins[p]++;
		}

		age++;

		for (size_t i = 0; i < organelles.size(); i ++)
		{
			organelles[i]->Event(terrain, x, y);
		}
	}

	RGBColor RationColor() override
	{
		return { 0, 143, 31 }; // green
	}

protected:



	RGBColor Species() override
	{
		return genom.Hash();
	}
	Cell* Reproduction()
	{
		CellSuccess isSuccess = CellSuccessRule(energy, age, max_age);
		float newMutationChanceK = 0;
		switch (isSuccess)
		{
		case Cell::CellSuccess::fail:
			newMutationChanceK = 0.01;
			break;
		case Cell::CellSuccess::good:
			newMutationChanceK = -0.01;
			break;
		}
		Genome new_genom = genom.Replicate(newMutationChanceK);

		short max_age_koef = 0;
		switch (isSuccess)
		{
		case Cell::CellSuccess::fail:
			max_age_koef = -1;
			break;
		case Cell::CellSuccess::good:
			max_age_koef = 1;
			break;
		}

		unsigned short new_max_age = max_age + max_age_koef;
		if (new_max_age > 1000) new_max_age = 1000;
		if (new_max_age < 2) new_max_age = 2;
		
		std::vector<Organelle*> new_organelles = organelles;

		return new Cell(
			100,
			new_max_age,
			separation_cost,
			birth_cost,
			defence,
			attack,
			new_genom,
			new_organelles);
	}

	enum class CellSuccess
	{
		fail,
		normal,
		good
	};

	CellSuccess CellSuccessRule(size_t accumulated_energy, unsigned short age, unsigned short max_age)
	{
		unsigned int success_border = MAX_ENERGY * (float(age) / (max_age / 2));
		return (accumulated_energy > success_border) ? CellSuccess::good :
			(accumulated_energy > (success_border / 2) ? CellSuccess::normal : CellSuccess::fail);
	}
};
