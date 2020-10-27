#pragma once
#include "cell.h"
#include "cell_predator.h"
#include "cell_herbivorous.h"
#include "structure.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

class CellMineravourous : public Cell
{
public:
	explicit CellMineravourous(unsigned short e, unsigned short max_age, size_t sepr_cost, size_t birthcost, double defence, unsigned short attack, Genome g)
		: Cell(energy, max_age, sepr_cost, birthcost, defence, attack, g) {}

	RGBColor RationColor() override
	{
		return { 0, 14, 140 }; // blue
	}

	void Eat(Structure* our_point, Structure*) override
	{
		DecreaceEnergy(10);
		auto e = our_point->GetFood().Eat(MAX_MINERALS_TO_EAT);
		if (e)
		{
			IncreaceEnergy(e);
		}
	}
private:
	Cell* Mutation(NewCellStat stat) override;
};
