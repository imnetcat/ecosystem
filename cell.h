#pragma once

#include "ration.h"
#include "entity.h"

#include <iostream>

class Cell : public Entity
{
private:
	unsigned short hp = 100;
	unsigned short energy = 100;
	unsigned short accumulated_energy = 0;
public:
	explicit Cell(Ration ph, Gen g)
		: Entity(ph, g){
		std::cout << "\t\tGen: "
			<< "\t" << g.data[0] << std::endl
			<< "\t" << g.data[1] << std::endl
			<< "\t" << g.data[2] << std::endl
			<< "\t" << g.data[3] << std::endl
			<< "\t" << g.data[4] << std::endl
			<< "\t" << g.data[5] << std::endl
			<< "\t" << g.data[6] << std::endl
			<< "\t" << g.data[7] << std::endl;
	}

	Ration GetRation() const override
	{
		return ration;
	}

	unsigned short Die() const override
	{
		return accumulated_energy + 100;
	}
	bool Dying() const override
	{
		return !hp;
	}

	Command Tic() override
	{
		std::cout << "\t\tAccumulatedy: " << accumulated_energy << std::endl;
		std::cout << "\t\tEnergy: " << energy << std::endl;
		std::cout << "\t\tHp: " << hp << std::endl;
		DecreaceEnergy(5);
		return genom.Read();
	}

	void Fotosintesis(unsigned short value) override
	{
		if(ration.fotosintesis < 32000)
			ration.fotosintesis++;

		IncreaceEnergy(value);
	}

	void IncreaceEnergy(unsigned short value) override
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
