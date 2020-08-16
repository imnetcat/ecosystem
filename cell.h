#pragma once
#include "entity.h"

#include <iostream>
#include <memory>
#include <ctime>

class Cell : public Entity
{
private:
	Gen genom;
public:
	explicit Cell(Gen g, ration r, unsigned short e)
		: Entity(100, 100, e, r), genom(g) {}
	
	Gen GetGen()
	{
		return genom;
	}
	
	bool IsFriendly(Gen genom1)
	{
		size_t counter = 0;
		for (size_t i = 0; i < Gen::length; i++)
		{
			if (genom1.data[i] != genom.data[i])
				counter++;
		}

		return counter < 2;
	}

	std::shared_ptr<Entity> Reproduction() override
	{
		srand(time(0));  // рандомизация генератора случайных чисел
		// вычисляем произойдёт ли мутация
		auto new_genom = genom.data;
		double mt = (rand() % 100) / 100;
		if (mt <= genom.mutationChance)
		{
			// рандомизируем выбор изменяемой комманды гена
			srand(time(0));
			size_t index = rand() % Gen::length;
			// рандомизируем комманду гена
			srand(time(0)); 
			new_genom[index] = static_cast<Command>(rand() % Gen::commands);
		}
		unsigned short hlph = accumulated_energy / 2;
		DecreaceAccEnergy(hlph);
		return std::shared_ptr<Entity>(new Cell(
			Gen(new_genom, 0.25, genom.generation + 1), ration_, hlph
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

	RGBColor Color(view_settings vs) override
	{
		switch (vs)
		{
		case view_settings::default:
			return { ration_.Meat(), ration_.Light(), ration_.Minerals() };
			break;
		case view_settings::energy:
			break;
		case view_settings::species:
			break;
		default:
			break;
		}
	}
		
	bool Outline() override
	{
		return true;
	}
};
