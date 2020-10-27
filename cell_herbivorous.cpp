#include "cell_herbivorous.h"
#include "cell_mineravorous.h"

Cell* CellHerbivorous::Mutation(NewCellStat stat)
{
	if (!stat.is_mutating)
	{
		return new CellHerbivorous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}

	unsigned short random = rand() % 100;
	if (random < 10)
	{
		return new CellMineravourous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
	else
	{
		return new CellHerbivorous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
}
