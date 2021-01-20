#pragma once
#include "map_terrain.h"

void Birthing(int args, size_t x, size_t y)
{
	if (terrain[y][x].GetCell().Energy() < (terrain[y][x].GetCell().ReprodutionCost() / 2))
		return;

	Position new_position = GetInvertedViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[new_position.y][new_position.x].SetCell();
		terrain[y][x].GetCell().DecreaceEnergy(terrain[y][x].GetCell().ReprodutionCost() / 2);
		terrain[y][x].GetCell().Reproduction(terrain[new_position.y][new_position.x].GetCell());
		cells[cells_count] = &terrain[new_position.y][new_position.x].GetCell();
		cells[cells_count]->SetPosition(new_position.x, new_position.y);

		size_t cost = 0;
		for (const Gen& gen : cells[cells_count]->GetGenome().data)
		{
			cost += CREATION_COST.at(gen.trigger);
		}
		cells[cells_count]->ReproductionCost(100 + cost);

		cells_count++;
	}
}
