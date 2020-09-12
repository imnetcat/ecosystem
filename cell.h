#pragma once
#include "entity.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

class Cell : public Entity
{
private:
	Genome genom;
	const size_t separation_cost;
	const size_t birth_cost;
public:
	Cell(Cell&& obj) :
		Entity(100, 100, std::move(obj.AccEnergy()), std::move(obj.MaxAge()), std::move(obj.ration_)),
		genom(std::move(obj.GetGenome())),
		separation_cost(std::move(obj.SeparationCost())),
		birth_cost(std::move(obj.BirthCost())) {}

	explicit Cell(unsigned short e, unsigned short max_age, size_t repr_cost, size_t birthcost, Genome g = Genome(), Ration r = Ration())
		: Entity(100, 100, e, max_age, r), genom(g), separation_cost(repr_cost), birth_cost(birthcost) {}
	
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
		const unsigned short BORDER = 2;
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
		Cell* new_cell = Reproduction();

		DecreaceAccEnergy(separation_cost);
		unsigned short hlph = accumulated_energy / 2;
		DecreaceAccEnergy(hlph);
		new_cell->IncreaceAccEnergy(hlph);

		return new_cell;
	}

	Cell* Birth()
	{
		return Reproduction();
	}
	
	void Tic(std::vector<Gen::Command>& commands) override
	{
		if (!hp || age == max_age)
			commands.push_back(Gen::Command::die);
		else
			commands.push_back(genom.Read());
		
		age++;
	}

	RGBColor Species() override
	{
		return genom.Hash();
	}

	RGBColor Color(view_settings vs) override
	{
		switch (vs)
		{
		case terrain:
			return { 0, 255, 226 };
		case view_settings::minerals:
			return { 209, 209, 209 };
		case view_settings::ration:
			return { ration_.Meat(), ration_.Light(), ration_.Minerals() };
		case view_settings::energy:
		{
			if (accumulated_energy < (MAX_ACC_ENERGY / 2))
			{
				return { 255, static_cast<unsigned char>(255 * (accumulated_energy / (double)(MAX_ACC_ENERGY / 2))), 0 };
			}
			else
			{
				return { static_cast<unsigned char>(255 - 255 * (accumulated_energy / (double)MAX_ACC_ENERGY)), 255, 0 };
			}
		}
		case view_settings::species:
			return Species();
		case view_settings::age:
		{
			unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)age / max_age));
			return { c, c, c };
		}
		default:
			return { 0, 255, 226 };
		}
	}
		
	bool Outline(view_settings) override
	{
		return true;
	}
private:
	Cell* Reproduction()
	{
		srand(time(0) + rand());  // рандомизация генератора случайных чисел
		   // вычисляем произойдёт ли мутация
		auto new_genom = genom.data;
		double mt = (rand() % 100) / (double)100;
		if (mt < genom.mutationChance)
		{
			// рандомизируем выбор изменяемой комманды гена
			size_t index = rand() % Genome::length;
			// рандомизируем комманду гена
			new_genom[index] = Gen();
		}

		short max_age_koef = CellSuccessRule(accumulated_energy, max_age, 1, -2);
		double mutationChance_koef = CellSuccessRule(accumulated_energy, max_age, -0.01, 0.01);

		unsigned short new_max_age = max_age + max_age_koef;
		double new_mutationChance = genom.mutationChance + mutationChance_koef;
		if (new_mutationChance > 1) new_mutationChance = 1;
		if (new_mutationChance < 0) new_mutationChance = 0;
		if (new_max_age > 50 * Genome::length) new_max_age = 50 * Genome::length;
		if (new_max_age < Genome::length) new_max_age = Genome::length;

		return new Cell(
			0, new_max_age, separation_cost, birth_cost, Genome(new_genom, new_mutationChance, genom.generation + 1), ration_
		);
	}
	double CellSuccessRule(size_t accumulated_energy, size_t max_age, double success, double fail)
	{
		return accumulated_energy > max_age * 10 ? success :
			(accumulated_energy > max_age / 2 * 10 ? 1 : fail);
	}
};
