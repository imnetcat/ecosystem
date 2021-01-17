#include "sleeping.h"
#include "structure.h"

void Sleeping::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (!HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	terrain[y][x]->GetCell()->Proteins()[Protein::sleepon]--;

	terrain[y][x]->GetCell()->DecreaceEnergy(1);
}
bool Sleeping::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.at(Protein::sleepon) > 1;
}
bool Sleeping::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::sleepon);
}