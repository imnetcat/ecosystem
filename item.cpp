#include "item.h"

Item::Item(bool ea, bool w) :
	walkable(w),
	eatable(ea) {}

RGBColor Item::MineralsColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::RationColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::EnergyColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::SpeciesColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::AgeColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::HpColor()
{
	return { 143, 229, 255 };
}
RGBColor Item::SurvivalColor()
{
	return { 143, 229, 255 };
}

void Item::Walkable(bool val)
{
	walkable = val;
}
bool Item::IsWalkable()
{
	return walkable;
}
bool Item::isEatable()
{
	return eatable;
}
