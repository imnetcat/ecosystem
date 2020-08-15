#pragma once
#include "item.h"
#include <map>
#include "ration.h"

class Food : public Item
{
protected:
	std::map<ration, unsigned short> energy;
	bool ticed;
public:
	Food() : Item(true, true),
		ticed(false) {}
	std::map<ration, unsigned short>& Get()
	{
		return energy;
	}

	bool Empty()
	{
		return energy.empty();
		for (auto& [r, e] : energy)
		{
			if (e)
				return false;
		}
		return true;
	}

	Food(ration r, unsigned short e)
		: Item(true, true) 
	{
		energy[r] = e;
	}

	void Put(ration r, unsigned short e)
	{
		energy[r] += e;
	}

	unsigned short Eat(ration r)
	{
		auto e = energy[r];
		energy[r] = 0;
		return e;
	}
	unsigned short Eat(ration r, unsigned short val)
	{
		if (energy[r] > val)
		{
			energy[r] -= val;
		}
		else
		{
			val -= energy[r];
			energy[r] = 0;
		}
		return val;
	}
	RGBColor Color() override
	{
		return	{ 0, 227, 208 };
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
