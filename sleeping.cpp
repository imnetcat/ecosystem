#include "sleeping.h"

void Sleeping::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::sleepon) < 1)
		return;

	terrain[y][x].GetCell().Proteins()[Protein::sleepon]--;
}
