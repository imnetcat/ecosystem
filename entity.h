#pragma once
#include "ration.h"
#include "object.h"

class Entity : public Object
{
protected:
	Ration ration;
	unsigned short age;
	unsigned short hp;
	unsigned short energy;
	unsigned int accumulated_energy;
public:
	Entity(Ration r, unsigned short h, unsigned short e) :
		age(0),
		hp(h),
		energy(e),
		accumulated_energy(0),
		ration(r) {}

	Ration GetRation() const
	{
		return ration;
	}

	virtual void Fotosintesis(unsigned short value) = 0;
	virtual std::shared_ptr<Entity> Furcation() = 0;

	RGBColor Color() override
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
		return RGBColor{ r, g, b };
	}

	void IncreaceAccEnergy(unsigned short value)
	{
		if (accumulated_energy < 32000)
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
