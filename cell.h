#pragma once
#include "entity.h"

#include <iostream>
#include <memory>
#include <ctime>

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

		DecreaceAccEnergy(reproduction_cost);
		unsigned short hlph = accumulated_energy / 2;
		DecreaceAccEnergy(hlph);
		return std::shared_ptr<Entity>(new Cell(
			hlph, reproduction_cost, max_age, Gen(new_genom, 0.25, genom.generation + 1), ration_
		));
	}
	
	void Tic(std::vector<Command>& commands) override
	{
		age++;
		if (!hp || age == MAX_CELL_AGE)
			commands.push_back(die);
		else
			commands.push_back(genom.Read());
	}

	RGBColor Species() override
	{
		unsigned char part1, part2;
		if (Gen::length % 3 == 0)
		{
			part1 = part2 = Gen::length / 3;
		}
		else if (Gen::length % 3 == 1)
		{
			part1 = part2 = Gen::length / 3;
		}
		else
		{
			part1 = Gen::length / 3;
			part2 = Gen::length / 3 + 1;
		}
		
		unsigned char r = 0, g = 0, b = 0;
		for (unsigned short i = 0; i < part1; i++)
		{
			unsigned char gen = static_cast<unsigned char>(genom.data[i]);
			r += gen * ((double)255 / part1 * Gen::commands);
		}
		for (unsigned short i = part1; i < part1 + part2; i++)
		{
			int gen = static_cast<unsigned char>(genom.data[i]);
			g += gen * ((double)255 / part1 * Gen::commands);
		}
		for (unsigned short i = part2 + part1; i < Gen::length; i++)
		{
			int gen = static_cast<unsigned char>(genom.data[i]);
			b += gen * ((double)255 / part1 * Gen::commands);
		}
		
		if (r > 255)
			r = r % 255;
		if (g > 255)
			g = g % 255;
		if (b > 255)
			b = b % 255;

		return { r, g, b };
	}

	RGBColor Color(view_settings vs) override
	{
		switch (vs)
		{
		case terrain:
			return { 0, 255, 226 };
			break;
		case view_settings::minerals:
			return { 209, 209, 209 };
			break;
		case view_settings::ration:
			return { ration_.Meat(), ration_.Light(), ration_.Minerals() };
			break;
		case view_settings::energy:
			return { 255, static_cast<unsigned char>(255 - 255 * (double)accumulated_energy / MAX_ACC_ENERGY), 0 };
			break;
		case view_settings::species:
		{
			return Species();
		}
			break;
		}
		return { ration_.Meat(), ration_.Light(), ration_.Minerals() };
	}
		
	bool Outline(view_settings) override
	{
		return true;
	}
};
