#include "TerrainCell.h"

EntityIterator TerrainCell::GetEntity()
{
	return entity;
}
void TerrainCell::SetEntity(EntityIterator value)
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

void TerrainCell::SetFood(size_t value)
{
	organic = value;
}
void TerrainCell::DelFood()
{
	organic = 0;
}
size_t TerrainCell::GetFood() const
{
	return organic;
}

bool TerrainCell::IsWalkable()
{
	return material.Walkable() && !ContainsEntity();
}

bool TerrainCell::IsContainsFood()
{
	return organic;
}
