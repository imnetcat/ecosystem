#include "separationing.h"
#include "structure.h"
void Separationing::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	if (terrain[y][x]->GetCell()->Energy() < terrain[y][x]->GetCell()->SeparationCost())
		return;

	Position new_position = GetViewedPosition(terrain[y][x]->GetCell()->GetView(), { x,y });

	if (!terrain[new_position.y][new_position.x]->IsContainsCell() && terrain[new_position.y][new_position.x]->IsWalkable())
	{
		terrain[new_position.y][new_position.x]->SetCell(terrain[y][x]->GetCell()->Separation());
	}

	terrain[y][x]->GetCell()->Proteins()[Protein::separation] -= 10;
}
bool Separationing::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::separation) > 10;
}
bool Separationing::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::separation);
}