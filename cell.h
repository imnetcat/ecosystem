#pragma once

#include "ration.h"
#include "entity.h"
#include "gen.h"

#include <iostream>
#include <memory>

class Cell : public Entity
{
private:
	Gen genom;
public:
	explicit Cell(Ration ph, Gen g)
		: Entity(ph, 100, 100), genom(g)
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
	
	std::shared_ptr<Entity> Furcation() override
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
				// рандомизируем рацион
				srand(time(0));
				switch (index)
				{
				case 0:
					ration.entities = ration.entities + rand() % 100;
					break;
				case 1:
					ration.fotosintesis = ration.fotosintesis + rand() % 100;
					break;
				case 2:
					ration.minerals = ration.minerals + rand() % 100;
					break;
				}
			}
			// рандомизируем выбор изменяемой ветви рациона
			srand(time(0));
			index = rand() % 3;
			// рандомизируем рацион
			srand(time(0));
			switch (index)
			{
			case 0:
				ration.entities = ration.entities + rand() % 100;
				break;
			case 1:
				ration.fotosintesis = ration.fotosintesis + rand() % 100;
				break;
			case 2:
				ration.minerals = ration.minerals + rand() % 100;
				break;
			}
			// рандомизируем рацион
			srand(time(0));
			switch (index)
			{
			case 0:
				ration.entities = ration.entities - rand() % 100;
				break;
			case 1:
				ration.fotosintesis = ration.fotosintesis - rand() % 100;
				break;
			case 2:
				ration.minerals = ration.minerals - rand() % 100;
				break;
			}
		}
		return std::shared_ptr<Entity>(new Cell(
			ration,
			Gen(new_genom, newMutationChance, genom.generation + 1)
		));
	}

	unsigned short Die() const override
	{
		return accumulated_energy + 100;
	}
	bool Dying() const override
	{
		return !hp;
	}

	void Tic(std::vector<Command>& commands) override
	{
		DecreaceEnergy(1);
		commands.push_back(genom.Read());
	}

	void Fotosintesis(unsigned short value) override
	{
		if(ration.fotosintesis < 32000)
			ration.fotosintesis++;

		IncreaceEnergy(value);
	}

	bool Outline() override
	{
		return true;
	}
};
