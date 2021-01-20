#pragma once
#include "map_terrain.h"

void Moving(int args, size_t x, size_t y)
{
	Position new_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (new_position == Position{ x, y })
		return;

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		size_t index;
		for (index = 0; index < cells.size(); index++)
		{
			if (cells[index]->GetX() == x &&
				cells[index]->GetY() == y)
			{
				cells[index] = &terrain[new_position.y][new_position.x].GetCell();
				break;
			}
		}
		terrain[y][x].GetCell().SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetCell(terrain[y][x].GetCell());
		
		terrain[y][x].DelCell();
	}
}
