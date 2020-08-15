#pragma once
#include "entity.h"
#include "gen.h"

#include <iostream>
#include <memory>
#include <ctime>

class CellPhotosynthetic : public Entity
{
private:
	Gen genom;
public:
	explicit CellPhotosynthetic(Gen g)
		: Entity(100, 100), genom(g)
	{
		std::cout << "\t\tGeneration: " << g.generation << std::endl;
		std::cout << "\t\tGenome: " << std::endl
			<< "\t" << g.data[0] << std::endl
			<< "\t" << g.data[1] << std::endl
			<< "\t" << g.data[2] << std::endl
			<< "\t" << g.data[3] << std::endl
			<< "\t" << g.data[4] << std::endl
			<< "\t" << g.data[5] << std::endl
			<< "\t" << g.data[6] << std::endl
			<< "\t" << g.data[7] << std::endl;
	}
	
	std::shared_ptr<Entity> Reproduction() override
	{
		// Стоимость деления клетки
		DecreaceAccEnergy(200);

		srand(time(0));  // рандомизация генератора случайных чисел
		size_t newMutationChance = 1 + rand() % 100;
		srand(time(0));  // рандомизация генератора случайных чисел
		auto new_genom = genom.data;
		size_t mutationIndex = rand() % genom.mutationChance;
		if (!mutationIndex)
		{
			// рандомизируем выбор изменяемой комманды гена
			srand(time(0));
			size_t index = rand() % Gen::length;
			// рандомизируем комманду гена
			srand(time(0)); 
			new_genom[index] = static_cast<Command>(rand() % Gen::commands);
			if (mutationIndex < 25)
			{
				srand(time(0));
				index = rand() % Gen::length;
				srand(time(0));
				new_genom[index] = static_cast<Command>(rand() % Gen::commands);
			}
			else if (mutationIndex < 50)
			{
				srand(time(0));
				index = rand() % Gen::length;
				srand(time(0));
				new_genom[index] = static_cast<Command>(rand() % Gen::commands);
			}
			else
			{
				srand(time(0));
				index = rand() % Gen::length;
				srand(time(0));
				new_genom[index] = static_cast<Command>(rand() % Gen::commands);
				srand(time(0));
				index = rand() % Gen::length;
				srand(time(0));
				new_genom[index] = static_cast<Command>(rand() % Gen::commands);
				srand(time(0));
				index = rand() % 3;
			}
		}
		return std::shared_ptr<Entity>(new CellPhotosynthetic(
			Gen(new_genom, newMutationChance, genom.generation + 1)
		));
	}
	
	void Tic(std::vector<Command>& commands) override
	{
		age++;
		DecreaceEnergy(ENERGY_PER_TIC);
		if (!hp || age == MAX_CELL_AGE)
			commands.push_back(die);
		else
			commands.push_back(genom.Read());
	}

	RGBColor Color() override
	{
		return { 0, 255, 0 };
	}

	ration Ration() override
	{
		return light;
	}

	void Eat(unsigned short energy) override
	{
		IncreaceEnergy(energy);
	}

	bool Outline() override
	{
		return true;
	}
};
