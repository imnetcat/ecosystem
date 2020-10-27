#pragma once
#include "cell.h"
#include "cell_mineravorous.h"
#include "structure.h"
#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>

class CellPredator : public Cell
{
public:
	explicit CellPredator(unsigned short e, unsigned short max_age, size_t sepr_cost, size_t birthcost, double defence, unsigned short attack, Genome g)
		: Cell(energy, max_age, sepr_cost, birthcost, defence, attack, g) {}

	RGBColor RationColor() override
	{
		return { 199, 0, 0 }; // red
	}
	void Eat(Structure* our_point, Structure* viewed_point) override
	{
		if (viewed_point->IsContainsEntity())
		{
			if (!viewed_point->GetEntity()->IsFriendly(this))
			{
				if (!viewed_point->GetEntity()->Defenced(Attack()))
				{
					AttackUp();
					auto e = viewed_point->GetEntity()->Energy();
					if(e > MAX_MEAT_TO_EAT)
						IncreaceEnergy(MAX_MEAT_TO_EAT);
					else
						IncreaceEnergy(e);
				}
			}
		}
	}
private:
	Cell* Mutation(NewCellStat stat) override;
};
