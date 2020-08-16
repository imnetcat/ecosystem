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

		unsigned short hlph = 0;
		if (accumulated_energy > 600)
		{
			hlph = 600;
			DecreaceAccEnergy(600);
		}
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
			return { 255, static_cast<unsigned char>(255 - 255 * (double)accumulated_energy / MAX_ACC_ENERGY), 0 };
			break;
		case view_settings::species:
		{
			unsigned char r = 0;
			for (unsigned short i = 0; i < Gen::length; i++)
			{
				r += static_cast<unsigned char>(genom.data[i]);
			}
			r = 255 - r;
			unsigned char g = 255 - r;
			unsigned char b = 255 - r;
			return { r, g, b };
		}
			break;
		}
		return { ration_.Meat(), ration_.Light(), ration_.Minerals() };
	}
		
	bool Outline() override
	{
		return true;
	}
};
