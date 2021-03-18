#include "cell.h"
using namespace Ecosystem::Logic;

void cell::Init(unsigned int lpower, double lcoef, size_t x, size_t y)
{
	light_power = lpower;
	light_coef = lcoef;
	SetPosition(x, y);
}

EntitiesIterator cell::GetEntity() const
{
	return entity;
}
void cell::SetEntity(EntitiesIterator value)
{
	entity = value;
	has_entity = true;
}
void cell::DelEntity()
{
	has_entity = false;
}
bool cell::ContainsEntity() const
{
	return has_entity;
}

void cell::SetOrganic(OrganicIterator value)
{
	has_object = true;
	object = value;
}
void cell::AddOrganic(size_t value)
{
	object->Increace(value);
}
void cell::DelOrganic()
{
	has_object = false;
}
OrganicIterator cell::GetOrganic() const
{
	return object;
}

bool cell::IsContainsOrganic() const
{
	return has_object;
}

unsigned int cell::LightPower() const
{
	return y() ? y() * light_coef * light_power : light_power;
}
