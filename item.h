#pragma once
#include "object.h"

class Item : public Object
{
protected:
	bool eatable;
	bool walkable;
public:
	Item(bool ea, bool w);

	Item& operator = (const Item& obj)
	{
		eatable = obj.eatable;
		walkable = obj.walkable;
		return *this;
	}
	Item& operator = (Item&& obj)
	{
		eatable = obj.eatable;
		walkable = obj.walkable;
		return *this;
	}

	RGBColor GenerationsColor();
	RGBColor MineralsColor();
	RGBColor RationColor();
	RGBColor EnergyColor();
	RGBColor SpeciesColor();
	RGBColor AgeColor();
	RGBColor HpColor();
	RGBColor SurvivalColor();

	void Walkable(bool val);
	bool IsWalkable();
	bool isEatable();
};
