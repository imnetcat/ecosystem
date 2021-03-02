#include "TerrainCell.h"

Entity& TerrainCell::GetEntity()
{
	return entity;
}
void TerrainCell::SetEntity(const Entity& c)
{
	entity = c;
	has_entity = true;
}
void TerrainCell::SetEntity()
{
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

void TerrainCell::SetFood(Minerals obj)
{
	food.Put(obj.Get());
}
void TerrainCell::SetFood(unsigned short energy)
{
	food.Put(energy);
}
void TerrainCell::DelFood()
{
	food.Clear();
}
Minerals& TerrainCell::GetFood()
{
	return food;
}
const Minerals& TerrainCell::GetFood() const
{
	return food;
}

bool TerrainCell::IsWalkable()
{
	return material.IsWalkable() && !ContainsEntity();
}

bool TerrainCell::IsContainsFood()
{
	return !food.Empty();
}
