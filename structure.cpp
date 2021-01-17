#include "structure.h"

Structure::Structure(RGBColor c, bool w, float tr) :
	ticed(false),
	color(c),
	walkable(w),
	transparency(tr),
	light_level(0),
	cell(nullptr)
{ }

void Structure::Untick()
{
	ticed = false;
	food.Ticed(false);
}

RGBColor Structure::TerrainColor()
{
	if (cell)
		return cell->TerrainColor();
	if (!food.Empty())
		return food.TerrainColor();
	return color;
}
RGBColor Structure::MineralsColor()
{
	if (!food.Empty())
		return food.MineralsColor();
	if (cell)
		return cell->MineralsColor();
	return { 209, 209, 209 };
}
RGBColor Structure::RationColor()
{
	if (cell)
		return cell->RationColor();
	return { 209, 209, 209 };
}
RGBColor Structure::EnergyColor()
{
	if (cell)
		return cell->EnergyColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SpeciesColor()
{
	if (cell)
		return cell->SpeciesColor();
	return { 209, 209, 209 };
}
RGBColor Structure::AgeColor()
{
	if (cell)
		return cell->AgeColor();
	return { 143, 229, 255 };
}
RGBColor Structure::HpColor()
{
	if (cell)
		return cell->HpColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SurvivalColor()
{
	if (cell)
		return cell->SurvivalColor();
	return { 209, 209, 209 };
}

void Structure::Tic(MapTerrain& terrain, size_t& x, size_t& y)
{
	if (ticed)
		return;

	if (!food.Empty())
		food.Ticed(true);
	if (cell)
		cell->Tic(terrain, x, y);
	ticed = true;
}

unsigned char Structure::GetLightLevel()
{
	return light_level;
}
void Structure::SetLightLevel(unsigned short lv)
{
	light_level = lv;
}
unsigned short Structure::GetLightPower()
{
	return light_power;
}
void Structure::SetLightPower(unsigned short lp)
{
	light_power = lp;
	float power = (float)light_power / LIGHT_POWER;
	light_level = 128 - power * 128;
}

bool Structure::Outline(view_settings vs)
{
	if (vs != view_settings::minerals && cell)
		return cell->Outline(vs);

	if (!food.Empty())
		return food.Outline(vs);

	return false;
}

void Structure::SetFood(Minerals obj)
{
	food.Ticed(true);
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
void Structure::CleanCell()
{
	cell = nullptr;
}
void Structure::SetCell(Cell* c)
{
	delete cell;
	cell = c;
}
void Structure::DelCell()
{
	delete cell;
	cell = nullptr;
}
Cell* Structure::GetCell()
{
	return cell;
}

void Structure::Walkable(bool val)
{
	walkable = val;
}
bool Structure::IsWalkable()
{
	return walkable;
}

bool Structure::IsContainsFood()
{
	return !food.Empty();
}
bool Structure::IsContainsCell()
{
	return cell != nullptr;
}
