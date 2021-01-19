#include "photosynthesing.h"

void Photosynthesing::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::photosyntheson) < 2)
		return;

	terrain[y][x].GetCell().IncreaceEnergy(LIGHT_POWER);
	terrain[y][x].GetCell().Proteins()[Protein::photosyntheson] -= 2;
}