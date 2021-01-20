#pragma once
#include "map_terrain.h"

void Mineraling(int args, size_t x, size_t y)
{
	terrain[y][x].GetCell().RationLevel(-1, -1, 5);
	auto e = terrain[y][x].GetFood().Eat(MAX_MINERALS_TO_EAT);
	if (e)
	{
		terrain[y][x].GetCell().IncreaceEnergy(e);
	}
}
