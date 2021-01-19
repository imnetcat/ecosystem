#include "moving.h"

void Moving::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::moveon) < 10)
		return;

	Position new_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (new_position == Position{ x, y })
		return;

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[y][x].GetCell().Proteins()[Protein::moveon] -= 10;
		terrain[y][x].GetCell().DecreaceEnergy(25);

		terrain[y][x].GetCell().SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetCell(terrain[y][x].GetCell());
		terrain[y][x].DelCell();
		y = new_position.y;
		x = new_position.x;
	}
}
