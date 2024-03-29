#include "cell.h"
using namespace ecosystem::logic;

void cell::Init(unsigned int lpower, size_t x, size_t y)
{
	light_power = lpower;
	set(x, y);
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

bool cell::ContainsOrganic() const
{
	return has_object;
}

unsigned int cell::LightPower() const
{
	return (light_power / std::pow(1.3, y()));
}

int cell::x() const
{
	return position::x();
}

int cell::y() const
{
	return position::y();
}