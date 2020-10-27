#include "cell_predator.h"
#include "cell_mineravorous.h"

Cell* CellPredator::Mutation(NewCellStat stat)
{
	if (!stat.is_mutating)
	{
		return new CellPredator(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}

	unsigned short random = rand() % 100;
	if (random < 30)
	{
		return new CellMineravourous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
	else
	{
		return new CellPredator(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
}
