#include "turning.h"
#include "structure.h"

void Turning::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	std::unordered_map<Protein, unsigned long>& proteins = terrain[y][x]->GetCell()->Proteins();

	proteins[Protein::turnon] -= 5;

	if (proteins.count(Protein::left) > proteins.count(Protein::right))
	{
		unsigned short old_side = terrain[y][x]->GetCell()->GetView();
		view_side new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
		terrain[y][x]->GetCell()->SetView(new_side);
		terrain[y][x]->GetCell()->DecreaceEnergy(8);
		proteins[Protein::left] -= 5;
	}
	else
	{
		unsigned short old_side = terrain[y][x]->GetCell()->GetView();
		view_side new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
		terrain[y][x]->GetCell()->SetView(new_side);
		terrain[y][x]->GetCell()->DecreaceEnergy(8);
		proteins[Protein::right] -= 5;
	}
}
bool Turning::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::turnon) > 5;
}
bool Turning::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::turnon);
}