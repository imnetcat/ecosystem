#include "mineraling.h"
#include "structure.h"

void Mineraling::Event(MapTerrain& terrain, size_t& x, size_t& y) const 
{
	if (!HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	terrain[y][x]->GetCell()->DecreaceEnergy(10);
	auto e = terrain[y][x]->GetFood().Eat(MAX_MINERALS_TO_EAT);
	if (e)
	{
		terrain[y][x]->GetCell()->IncreaceEnergy(e);
		terrain[y][x]->GetCell()->Proteins()[Protein::mineraleon] -= 5;
	}
}
bool Mineraling::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.at(Protein::mineraleon) > 5;
}
bool Mineraling::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::mineraleon);
}