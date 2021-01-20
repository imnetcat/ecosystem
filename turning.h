#pragma once
#include "map_terrain.h"

void Turning(int args, size_t x, size_t y)
{
	unsigned short old_side = static_cast<unsigned short>(terrain[y][x].GetCell().GetView());
	view_side new_side;
	if (args % 2)
	{
		new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
	}
	else
	{
		new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
	}
	terrain[y][x].GetCell().SetView(new_side);
}
