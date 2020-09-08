#pragma once
#include "entity.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

class Cell : public Entity
{
private:
	Gen genom;
	const size_t reproduction_cost;
public:
	explicit Cell(unsigned short e, unsigned short max_age, size_t repr_cost, Gen g = Gen(), Ration r = Ration())
		: Entity(100, 100, e, max_age, r), genom(g), reproduction_cost(repr_cost) {}
	
	size_t ReproductionCost()
	{
		return reproduction_cost;
	}

	Gen GetGen()
	{
		return genom;
	}
	
	bool IsFriendly(std::shared_ptr<Entity>& cell)
	{
		const unsigned short BORDER = 2;
		size_t count_of_non_equal = 0;
		auto lhs_genom = GetGen().data;
		auto rhs_genom = cell->GetGen().data;

		for (size_t index = 0; index < lhs_genom.size(); index++)
		{
			if (lhs_genom[index] != rhs_genom[index])
				count_of_non_equal++;

			if (count_of_non_equal > BORDER)
				return false;
		}

		return true;
	}

	std::shared_ptr<Entity> Reproduction() override
	{
		srand(time(0));  // рандомизация генератора случайных чисел
		// вычисляем произойдёт ли мутация
		auto new_genom = genom.data;
		double mt = (rand() % 100) / (double)100;
		if (mt < genom.mutationChance)
		{
			// рандомизируем выбор изменяемой комманды гена
			size_t index = rand() % Gen::length;
			// рандомизируем комманду гена
			new_genom[index] = static_cast<Command>(rand() % Gen::commands);
		}

		const auto CellSuccessRule = [](size_t accumulated_energy, size_t max_age, double success, double fail) {
			return accumulated_energy > max_age * 10 ? success :
				(accumulated_energy > max_age / 2 * 10 ? 1 : fail);
		};

		double max_age_koef			= CellSuccessRule(accumulated_energy, max_age, 1.025, 0.0975);
		double mutationChance_koef	= CellSuccessRule(accumulated_energy, max_age, 0.9, 1.1);

		double new_max_age = max_age * max_age_koef;
		double new_mutationChance = genom.mutationChance * mutationChance_koef;
		if (new_mutationChance > 1) new_mutationChance = 1;

		DecreaceAccEnergy(reproduction_cost);
		unsigned short hlph = accumulated_energy / 2;
		DecreaceAccEnergy(hlph);
		return std::shared_ptr<Entity>(new Cell(
			hlph, max_age, reproduction_cost, Gen(new_genom, new_mutationChance, genom.generation + 1), ration_
		));
	}
	
	void Tic(std::vector<Command>& commands) override
	{
		if (!hp || age == max_age)
			commands.push_back(die);
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
			return { 255, static_cast<unsigned char>(255 - 255 * (double)accumulated_energy / MAX_ACC_ENERGY), 0 };
		case view_settings::species:
			return Species();
		}
	}
		
	bool Outline(view_settings) override
	{
		return true;
	}
};
