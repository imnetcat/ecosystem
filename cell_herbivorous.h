#pragma once
#include "cell.h"
#include "cell_mineravorous.h"
#include "structure.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

class CellHerbivorous : public Cell
{
public:
	explicit CellHerbivorous(unsigned short energy, unsigned short max_age, size_t sepr_cost, size_t birthcost, double defence, unsigned short attack, Genome g)
		: Cell(energy, max_age, sepr_cost, birthcost, defence, attack, g) {}
	
	RGBColor RationColor() override
	{
		return { 0, 143, 31 }; // green
	}

	void Eat(Structure* our_point, Structure*) override
	{
		DecreaceEnergy(10);
		auto e = our_point->GetLightPower();
		if (e)
		{
			if (e > MAX_LIGHT_TO_EAT)
				IncreaceEnergy(MAX_LIGHT_TO_EAT);
			else
				IncreaceEnergy(e);
		}
	}

protected:
	Cell* Mutation(NewCellStat stat) override;
};
