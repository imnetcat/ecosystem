#pragma once
#include "item.h"


class Minerals : public Item
{
protected:
	unsigned short energy;
	bool ticed;
public:
	Minerals() : Item(true, true),
		ticed(false), energy(0) {}
	Minerals(unsigned short e)
		: Item(true, true)
	{
		energy = e;
	}

	unsigned short Get()
	{
		return energy;
	}

	bool Empty()
	{
		return !energy;
	}

	void Put(unsigned short e)
	{
		energy += e;
	}

	void Clear()
	{
		energy = 0;;
	}

	unsigned short Eat()
	{
		auto e = energy;
		energy = 0;
		return e;
	}
	unsigned short Eat(unsigned short val)
	{
		if (energy > val)
		{
			energy -= val;
		}
		else
		{
			val -= energy;
			energy = 0;
		}
		return val;
	}
	RGBColor Color(view_settings) override
	{
		return	{ 209, 209, 209 };
	}
	bool Outline() override
	{
		return false;
	}
	void Ticed(bool val)
	{
		ticed = val;
	}
	bool Ticed()
	{
		return ticed;
	}
};
