#include "carnivorousing.h"

void Carnivorousing::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::carnivorouson) < 10)
		return;

	Position viewed_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (viewed_position == Position{ x, y })
		return;
	auto& viewed_point = terrain[viewed_position.y][viewed_position.x];

	if (viewed_point.ContainsCell())
	{
		if (!viewed_point.GetCell().IsFriendly(terrain[y][x].GetCell()))
		{
			if (!viewed_point.GetCell().Defenced(terrain[y][x].GetCell().Attack()))
			{
				terrain[y][x].GetCell().AttackUp();
				auto e = viewed_point.GetCell().Energy();
				if (e > MAX_MEAT_TO_EAT)
					terrain[y][x].GetCell().IncreaceEnergy(MAX_MEAT_TO_EAT);
				else
					terrain[y][x].GetCell().IncreaceEnergy(e);
				terrain[y][x].GetCell().Proteins()[Protein::carnivorouson] -= 10;
			}
		}
	}
}
