#include "moving.h"
#include "structure.h"

void Moving::Event(MapTerrain& terrain, size_t& x, size_t& y) const
{
	if (HaveRequestProteins(terrain[y][x]->GetCell()->Proteins()))
		return;

	Position new_position = GetViewedPosition(terrain[y][x]->GetCell()->GetView(), { x, y });

	if (new_position == Position{ x, y })
		return;

	if (terrain[new_position.y][new_position.x]->IsWalkable() && !terrain[new_position.y][new_position.x]->IsContainsCell())
	{
		terrain[y][x]->GetCell()->Proteins()[Protein::moveon] -= 10;
		terrain[y][x]->GetCell()->DecreaceEnergy(25);

		terrain[new_position.y][new_position.x]->SetCell(terrain[y][x]->GetCell());
		terrain[y][x]->CleanCell();
		y = new_position.y;
		x = new_position.x;
	}
}
bool Moving::HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::moveon) > 10;
}
bool Moving::IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const
{
	return proteins.count(Protein::moveon);
}