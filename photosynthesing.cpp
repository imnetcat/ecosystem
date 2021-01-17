#include "photosynthesing.h"
#include "structure.h"

void Photosynthesing::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (!HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	terrain[y][x]->GetCell()->DecreaceEnergy(10);
	auto e = terrain[y][x]->GetLightPower();
	if (e)
	{
		if (e > MAX_LIGHT_TO_EAT)
			terrain[y][x]->GetCell()->IncreaceEnergy(MAX_LIGHT_TO_EAT);
		else
			terrain[y][x]->GetCell()->IncreaceEnergy(e);
		terrain[y][x]->GetCell()->Proteins()[Protein::photosyntheson] -= 2;
	}
}
bool Photosynthesing::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.at(Protein::photosyntheson) > 2;
}
bool Photosynthesing::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::photosyntheson);
}