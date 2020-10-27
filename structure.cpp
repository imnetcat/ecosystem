#include "structure.h"

Structure::Structure(RGBColor c, bool w, double tr) :
	ticed(false),
	color(c),
	walkable(w),
	transparency(tr),
	light_level(0),
	contains_entity(false),
	entity(nullptr)
{ }

double Structure::Transparency()
{
	const double entity_transparency = 0.99;
	if (contains_entity)
	{
		return entity_transparency;
	}

	return transparency;
}

void Structure::Untick()
{
	ticed = false;
	food.Ticed(false);
}

RGBColor Structure::TerrainColor()
{
	if (contains_entity)
		return entity->TerrainColor();
	if (!food.Empty())
		return food.TerrainColor();
	return color;
}
RGBColor Structure::MineralsColor()
{
	if (!food.Empty())
		return food.MineralsColor();
	if (contains_entity)
		return entity->MineralsColor();
	return { 209, 209, 209 };
}
RGBColor Structure::RationColor()
{
	if (contains_entity)
		return entity->RationColor();
	return { 209, 209, 209 };
}
RGBColor Structure::EnergyColor()
{
	if (contains_entity)
		return entity->EnergyColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SpeciesColor()
{
	if (contains_entity)
		return entity->SpeciesColor();
	return { 209, 209, 209 };
}
RGBColor Structure::AgeColor()
{
	if (contains_entity)
		return entity->AgeColor();
	return { 143, 229, 255 };
}
RGBColor Structure::HpColor()
{
	if (contains_entity)
		return entity->HpColor();
	return { 209, 209, 209 };
}
RGBColor Structure::SurvivalColor()
{
	if (contains_entity)
		return entity->SurvivalColor();
	return { 209, 209, 209 };
}

void Structure::Tic(std::vector<Gen::Command>& commands)
{
	if (ticed)
		return;

	if (!food.Empty())
		food.Ticed(true);
	if (contains_entity)
		entity->Tic(commands);
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
	if (vs != view_settings::minerals && contains_entity)
		return entity->Outline(vs);

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

void Structure::ClearEntity()
{
	entity = nullptr;
	contains_entity = false;
}

void Structure::SetEntity(Cell* cell)
{
	if(entity)
		delete entity;
	entity = std::move(cell);
	contains_entity = true;
}
void Structure::DelEntity()
{
	if (entity)
		delete entity;
	ClearEntity();
}
Cell* Structure::GetEntity()
{
	return entity;
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
bool Structure::IsContainsEntity()
{
	return contains_entity;
}
