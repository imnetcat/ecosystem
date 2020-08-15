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
	explicit Cell(Gen g, unsigned short e)
		: Entity(100, 100, e), genom(g) {}
	
	Gen GetGen()
	{
		return genom;
	}

	std::shared_ptr<Entity> Reproduction() override
	{
		srand(time(0));  // рандомизация генератора случайных чисел
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
			Gen(new_genom, 0.2, genom.generation + 1), hlph
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

	RGBColor Color() override
	{
		return { 0, 255, 0 };
	}
		
	bool Outline() override
	{
		return true;
	}
};
