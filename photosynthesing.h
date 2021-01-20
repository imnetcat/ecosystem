#pragma once
#include "map_terrain.h"

void Photosynthesing(int args, size_t x, size_t y)
{
	terrain[y][x].GetCell().RationLevel(-1, 5, -1);
	terrain[y][x].GetCell().IncreaceEnergy(LIGHT_POWER);
}