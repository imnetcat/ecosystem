#include "turning.h"

void Turning::Event(size_t& x, size_t& y) const
{
	if (terrain[y][x].GetCell().Proteins().at(Protein::turnon) < 5)
		return;

	std::unordered_map<Protein, unsigned long>& proteins = terrain[y][x].GetCell().Proteins();

	proteins[Protein::turnon] -= 5;

	unsigned short old_side = static_cast<unsigned short>(terrain[y][x].GetCell().GetView());
	view_side new_side;
	if (proteins.count(Protein::left) > proteins.count(Protein::right))
	{
		new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
	}
	else
	{
		new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
	}
	terrain[y][x].GetCell().SetView(new_side);
	terrain[y][x].GetCell().DecreaceEnergy(8);
	proteins[Protein::right] -= 5;
}
