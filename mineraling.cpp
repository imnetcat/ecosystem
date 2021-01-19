#include "mineraling.h"

void Mineraling::Event(size_t& x, size_t& y) const 
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::mineraleon) < 5)
		return;

	terrain[y][x].GetCell().DecreaceEnergy(10);
	auto e = terrain[y][x].GetFood().Eat(MAX_MINERALS_TO_EAT);
	if (e)
	{
		terrain[y][x].GetCell().IncreaceEnergy(e);
		terrain[y][x].GetCell().Proteins()[Protein::mineraleon] -= 5;
	}
}
