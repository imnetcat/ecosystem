#include "birthing.h"

void Birthing::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::birthon) < 10)
		return;

	if (terrain[y][x].GetCell().Energy() < terrain[y][x].GetCell().BirthCost())
		return;

	Position new_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[y][x].GetCell().Birth(terrain[new_position.y][new_position.x].GetCell());
		cells[cells_count] = &terrain[new_position.y][new_position.x].GetCell();
		cells[cells_count]->SetPosition(new_position.x, new_position.y);
		cells_count++;
	}

	terrain[y][x].GetCell().Proteins()[Protein::birthon] -= 10;
}
