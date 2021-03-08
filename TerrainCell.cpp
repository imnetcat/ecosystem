#include "TerrainCell.h"

EntitiesIterator TerrainCell::GetEntity()
{
	return entity;
}
void TerrainCell::SetEntity(EntitiesIterator value)
{
	entity = value;
	has_entity = true;
}
void TerrainCell::DelEntity()
{
	has_entity = false;
}
bool TerrainCell::ContainsEntity() const
{
	return has_entity;
}

void TerrainCell::SetOrganic(OrganicIterator value)
{
	has_object = true;
	object = value;
}
void TerrainCell::AddOrganic(size_t value)
{
	object->Increace(value);
}
void TerrainCell::DelOrganic()
{
	has_object = false;
}
OrganicIterator TerrainCell::GetOrganic() const
{
	return object;
}

bool TerrainCell::IsContainsOrganic() const
{
	return has_object;
}

bool TerrainCell::IsWalkable()
{
	return !ContainsEntity();
}
