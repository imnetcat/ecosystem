#include "birthing.h"

void Birthing::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::birthon) < 10)
		return;

	if (terrain[y][x].GetCell().Energy() < (terrain[y][x].GetCell().ReprodutionCost() / 2))
		return;

	Position new_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[y][x].GetCell().DecreaceEnergy(terrain[y][x].GetCell().ReprodutionCost() / 2);
		terrain[y][x].GetCell().Reproduction(terrain[new_position.y][new_position.x].GetCell());
		cells[cells_count] = &terrain[new_position.y][new_position.x].GetCell();
		cells[cells_count]->SetPosition(new_position.x, new_position.y);
		
		size_t cost = 0;
		for (auto trigger : cells[cells_count]->Organelles())
		{
			cost += CREATION_COST.at(trigger);
		}
		cells[cells_count]->ReproductionCost(100 + cost);

		cells_count++;
	}

	terrain[y][x].GetCell().Proteins()[Protein::birthon] -= 10;
}
