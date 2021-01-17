#include "staying.h"
#include "structure.h"

void Staying::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (!HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	terrain[y][x]->GetCell()->Proteins()[Protein::stayon]--;

	terrain[y][x]->GetCell()->DecreaceEnergy(3);
}
bool Staying::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.at(Protein::stayon) > 1;
}
bool Staying::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::stayon);
}