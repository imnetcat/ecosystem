#pragma once
#include "object.h"

class Item : public Object
{
protected:
	const bool eatable;
	bool walkable;
public:
	Item(bool ea, bool w);


	RGBColor MineralsColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor RationColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor EnergyColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor SpeciesColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor AgeColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor HpColor()
	{
		return { 143, 229, 255 };
	}
	RGBColor SurvivalColor()
	{
		return { 143, 229, 255 };
	}

	void Walkable(bool val);
	bool IsWalkable();
	bool isEatable();
};
