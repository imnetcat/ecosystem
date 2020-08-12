#pragma once

#include "entity.h"

class Cell : public Entity
{
private:
	unsigned short hp = 100;
	unsigned short energy = 100;
public:
	explicit Cell(Ration ph, Gen g)
		: Entity(ph, g){}

	Ration Ration()
	{
		return ration;
	}

	unsigned short Die() override
	{
		return 100;
	}
	bool Dying() override
	{
		return !hp;
	}

	Command Tic() override
	{
		DecreaceEnergy(1);
		return genom.Read();
	}

	void IncreaceEnergy(unsigned short value)
	{
		energy += value;
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
