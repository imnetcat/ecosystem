#include "cell_herbivorous.h"
#include "cell_mineravorous.h"
#include "cell_predator.h"

Cell* CellMineravourous::Mutation(NewCellStat stat)
{
	if (!stat.is_mutating)
	{
		return new CellMineravourous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}

	unsigned short random = rand() % 100;
	if (random < 10)
	{
		return new CellPredator(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
	else if(random < 30)
	{
		return new CellHerbivorous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
	else
	{
		return new CellMineravourous(
			stat.energy, stat.max_age, stat.separation_cost, stat.birth_cost, stat.defence, stat.attack, stat.genom
		);
	}
}
