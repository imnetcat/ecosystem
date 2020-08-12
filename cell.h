#pragma once

#include "ration.h"
#include "object.h"
#include "gen.h"

#include <iostream>
#include <memory>

enum view_point
{
	left,
	top,
	right,
	bottom
};

class Cell : public Object
{
private:
	Ration ration;
	Gen genom;
	unsigned short hp = 100;
	unsigned short energy = 100;
	unsigned short accumulated_energy = 0;
	view_point vpoint = top;
public:
	explicit Cell(Ration ph, Gen g)
		: ration(ph), genom(g)
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
	ObjectColor Color() override
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		for (unsigned short i = ration.entities; i > 1; i--)
		{
			if (r != 250)
				r += 50;

			g = 0;
			b = 0;
		}
		for (unsigned short i = ration.fotosintesis; i > 1; i--)
		{
			if (g == 250 && r != 0)
				r -= 50;
			if (g != 250)
				g += 50;
			b = 0;
		}
		for (unsigned short i = ration.minerals; i > 1; i--)
		{
			if (r != 250)
				r += 50;
			if (g != 250)
				g += 50;
			b = 0;
		}
		return ObjectColor{ r, g, b };
	}
	std::shared_ptr<Cell> Furcation()
	{
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
		return std::shared_ptr<Cell>(new Cell(
			ration,
			Gen(new_genom, newMutationChance, genom.generation + 1)
		));
	}
	view_point GetViewPoint() const
	{
		return vpoint;
	}
	void SetViewPoint(view_point new_val)
	{
		vpoint = new_val;
	}

	Ration GetRation() const
	{
		return ration;
	}

	unsigned short Die() const
	{
		return accumulated_energy + 100;
	}
	bool Dying() const
	{
		return !hp;
	}

	Command Tic()
	{
		DecreaceEnergy(1);
		return genom.Read();
	}

	void Fotosintesis(unsigned short value)
	{
		if(ration.fotosintesis < 32000)
			ration.fotosintesis++;

		IncreaceEnergy(value);
	}

	void IncreaceAccEnergy(unsigned short value)
	{
		if(accumulated_energy < 32000)
			accumulated_energy += value;
	}
	void DecreaceAccEnergy(unsigned short value)
	{
		if (accumulated_energy > value)
			accumulated_energy -= value;
	}

	unsigned short AccEnergy()
	{
		return accumulated_energy;
	}

	void IncreaceEnergy(unsigned short value)
	{
		if (hp < 100)
		{
			IncreaceHp(value);
		}
		else
		{
			if (energy + value > 100)
			{
				accumulated_energy += value + energy - 100;
				energy = 100;
			}
			else
				energy += value;
		}
	}
	void DecreaceEnergy(unsigned short value)
	{
		if (energy > value)
		{
			energy -= value;
		}
		else if (energy > 0)
		{
			DecreaceHp(value - energy);
			energy = 0;
		}
		else
		{
			DecreaceHp(value);
		}
	}
	void IncreaceHp(unsigned short value)
	{
		if (hp + value > 100)
			hp = 100;
		else
			hp += value;
	}
	void DecreaceHp(unsigned short value)
	{
		if (hp > value)
		{
			hp -= value;
		}
		else
		{
			hp = 0;
		}
	}
	unsigned short Energy()
	{
		return energy;
	}
	unsigned short Hp()
	{
		return hp;
	}

};
