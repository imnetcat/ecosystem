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

RGBColor Structure::Color()
{
	return color;
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
	return walkable && !ContainsCell();
}

bool Structure::IsContainsFood()
{
	return !food.Empty();
}
