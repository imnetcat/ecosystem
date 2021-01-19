#include "structure.h"

Structure::Structure(RGBColor c, bool w, float tr) :
	color(c),
	walkable(w)
{ }

Structure& Structure::operator = (const Structure& structure)
{
	color = structure.color;
	walkable = structure.walkable;
	food = structure.food;
	cell = structure.cell;
	return *this;
}
Structure& Structure::operator = (Structure&& structure)
{
	color = structure.color;
	walkable = structure.walkable;
	food = structure.food;
	cell = structure.cell;
	return *this;
}

Cell& Structure::GetCell()
{
	return cell;
}
void Structure::SetCell(const Cell& c)
{
	cell = c;
	has_cell = true;
}
void Structure::SetCell()
{
	has_cell = true;
}
void Structure::DelCell()
{
	has_cell = false;
}
bool Structure::ContainsCell() const
{
	return has_cell;
}

RGBColor Structure::TerrainColor()
{
	if(has_cell)
		return cell.TerrainColor();
	if (!food.Empty())
		return food.TerrainColor();
	return color;
}
RGBColor Structure::MineralsColor()
{
	if (has_cell)
		return cell.MineralsColor();
	if (!food.Empty())
		return food.MineralsColor();
	return { 209, 209, 209 };
}
RGBColor Structure::RationColor()
{
	if (has_cell)
		return cell.RationColor();
	return { 209, 209, 209 };
}
RGBColor Structure::EnergyColor()
{
	if (has_cell)
		return cell.EnergyColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SpeciesColor()
{
	if (has_cell)
		return cell.SpeciesColor();
	return { 209, 209, 209 };
}
RGBColor Structure::AgeColor()
{
	if (has_cell)
		return cell.AgeColor();
	return { 143, 229, 255 };
}
RGBColor Structure::HpColor()
{
	if (has_cell)
		return cell.HpColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SurvivalColor()
{
	if (has_cell)
		return cell.SurvivalColor();
	return { 209, 209, 209 };
}

bool Structure::Outline(view_settings vs)
{
	if (has_cell)
		return cell.Outline(vs);
	if (!food.Empty())
		return food.Outline(vs);

	return false;
}

void Structure::SetFood(Minerals obj)
{
	food.Put(obj.Get());
}
void Structure::SetFood(unsigned short energy)
{
	food.Put(energy);
}
void Structure::DelFood()
{
	food.Clear();
}
Minerals& Structure::GetFood()
{
	return food;
}
const Minerals& Structure::GetFood() const
{
	return food;
}

void Structure::Walkable(bool val)
{
	walkable = val;
}
bool Structure::IsWalkable()
{
	return walkable && ContainsCell();
}

bool Structure::IsContainsFood()
{
	return !food.Empty();
}
