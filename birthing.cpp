#include "birthing.h"
#include "structure.h"

void Birthing::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (!HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	if (terrain[y][x]->GetCell()->Energy() < terrain[y][x]->GetCell()->BirthCost())
		return;

	Position new_position = GetViewedPosition(terrain[y][x]->GetCell()->GetView(), { x, y });

	if (!terrain[new_position.y][new_position.x]->IsContainsCell() && terrain[new_position.y][new_position.x]->IsWalkable())
	{
		terrain[new_position.y][new_position.x]->SetCell(terrain[y][x]->GetCell()->Birth());
	}

	terrain[y][x]->GetCell()->Proteins()[Protein::birthon] -= 10;
}
bool Birthing::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.at(Protein::birthon) > 10;
}
bool Birthing::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::birthon);
}