#include "staying.h"

void Staying::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::stayon) < 1)
		return;

	terrain[y][x].GetCell().Proteins()[Protein::stayon]--;

	terrain[y][x].GetCell().DecreaceEnergy(3);
}
