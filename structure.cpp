#include "structure.h"

Structure::Structure(RGBColor c, bool w, double tr) :
	ticed(false),
	color(c),
	walkable(w),
	transparency(tr),
	light_level(0),
	contains_entity(false)
{ }

double Structure::Transparency()
{
	double entity_transparency = 0.7;
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

RGBColor Structure::Color(view_settings vs)
{
	if (contains_entity)
		return entity->Color(vs);

	if (!food.Empty())
		return food.Color(vs);

	switch (vs)
	{
	case default:
		return color;
		break;
	case energy:
		return { 209, 209, 209 };
		break;
	case species:
		return { 209, 209, 209 };
		break;
	}
}

void Structure::Tic(std::vector<Command>& commands)
{
	if (ticed)
		return;

	if (!food.Empty())
		food.Ticed(true);
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
	float power = (float)light_power / LIGHT_POWER;
	light_level = 128 - power * 128;
}

bool Structure::Outline()
{
	if (contains_entity)
		return entity->Outline();
	if (!food.Empty())
		return food.Outline();

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

void Structure::SetEntity(std::shared_ptr<Entity> e)
{
	entity = e;
	contains_entity = true;
}
void Structure::DelEntity()
{
	entity = nullptr;
	contains_entity = false;
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
