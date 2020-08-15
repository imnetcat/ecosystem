#include "structure.h"

Structure::Structure(RGBColor c, bool w, unsigned short lv, double tr) :
	ticed(false),
	color(c),
	walkable(w),
	transparency(tr),
	light_level(lv),
	default_walkable(walkable),
	contains_entity(false)
{ }

double Structure::Transparency()
{
	return transparency;
}

void Structure::Untick()
{
	ticed = false;
	food.Untick();
}

RGBColor Structure::Color()
{
	if (contains_entity)
		return entity->Color();

	if (!food.Empty())
		return food.Color();

	return color;
}

void Structure::Tic(std::vector<Command>& commands)
{
	if (ticed)
		return;

	if (!food.Empty())
		food.Tic(commands);
	if (contains_entity)
		entity->Tic(commands);
	ticed = true;
}

unsigned short Structure::GetLightLevel()
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
}

bool Structure::Outline()
{
	if (contains_entity)
		return entity->Outline();
	if (!food.Empty())
		return food.Outline();

	return false;
}

void Structure::SetFood(Food obj)
{
	auto& obj_energy = obj.Get();
	for (auto&[r, energy] : obj_energy)
	{
		food.Put(r, energy);
	}
}
void Structure::SetFood(ration r, unsigned short energy)
{
	food.Put(r, energy);
}
void Structure::DelFood()
{
	food.Get().clear();
	walkable = default_walkable;
}
Food& Structure::GetFood()
{
	return food;
}
const Food& Structure::GetFood() const
{
	return food;
}

void Structure::SetEntity(std::shared_ptr<Entity> e)
{
	entity = e;
	contains_entity = true;
	walkable = false;
}
void Structure::DelEntity()
{
	entity = nullptr;
	contains_entity = false;
	walkable = default_walkable;
}
std::shared_ptr<Entity>& Structure::GetEntity()
{
	return entity;
}

void Structure::Walkable(bool val)
{
	walkable = val;
}
bool Structure::IsWalkable()
{
	if (!food.Empty())
		return (walkable || food.IsWalkable());

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
