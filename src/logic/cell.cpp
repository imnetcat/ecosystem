#include "cell.h"

EntitiesIterator cell::GetEntity()
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
