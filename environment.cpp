#include "environment.h"

size_t Environment::cells_count = 0;
size_t Environment::max_generation = 0;
std::array<std::array<Structure, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> Environment::terrain;
std::array<Cell*, ENVIRONMENT_SIZE_X* ENVIRONMENT_SIZE_Y> Environment::cells;

Environment::Environment()
{
	size_t index = 0;
	srand(time(0) + rand());
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			// fill of water
			terrain[y][x] = Water();
			// fill of nullptr
			cells[index] = nullptr;

			// put minerals
			if (((rand() % 100) < 25))
			{
				terrain[y][x].SetFood(100);
			}

			// put first cells
			if (((rand() % 100) < 1) && cells_count < CELL_START_COUNT)
			{
				terrain[y][x].SetCell();
				cells[cells_count] = &terrain[y][x].GetCell();
				cells[cells_count]->SetPosition(x, y);
				cells[cells_count]->Energy(100);
				cells[cells_count]->MaxAge(100);
				cells[cells_count]->Defence(0.01);
				cells[cells_count]->Hp(MAX_HP);
				cells[cells_count]->Attack(0.01);
				cells[cells_count]->SetGenome(Genome());

				size_t cost = 0;
				for (const Gen& gen : cells[cells_count]->GetGenome().data)
				{
					cost += CREATION_COST.at(gen.trigger);
				}
				cells[cells_count]->ReproductionCost(100 + cost);

				cells_count++;
			}

			index++;
		}
	}
}
void Environment::Shift(size_t i)
{
	size_t index;
	for (index = i + 1; index < cells.size(); index++)
	{
		cells[index - 1] = cells[index];
		if (!cells[index]) break;
	}
	if (cells_count)
	{
		cells_count--;
	}
}

void Environment::CellDie(size_t index, size_t x, size_t y)
{
	if (terrain[y][x].IsContainsFood())
		terrain[y][x].GetFood().Put(cells[index]->Energy() + 100);
	else
		terrain[y][x].SetFood(cells[index]->Energy() + 100);

	terrain[y][x].DelCell();
	Shift(index);
}
void Environment::CellDie(size_t x, size_t y)
{
	size_t index;
	for (index = 0; index < cells.size(); index++)
	{
		if (cells[index]->GetX() == x &&
			cells[index]->GetY() == y)
			break;
	}

	if (index == cells.size()) return;

	if (terrain[y][x].IsContainsFood())
		terrain[y][x].GetFood().Put(cells[index]->Energy() + 100);
	else
		terrain[y][x].SetFood(cells[index]->Energy() + 100);

	terrain[y][x].DelCell();
	Shift(index);
}
void Environment::Separationing(int args, size_t x, size_t y)
{
	if (terrain[y][x].GetCell().Energy() < terrain[y][x].GetCell().ReprodutionCost())
		return;

	Position new_position = GetInvertedViewedPosition(terrain[y][x].GetCell().GetView(), { x,y });

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[new_position.y][new_position.x].SetCell();
		terrain[y][x].GetCell().Reproduction(terrain[new_position.y][new_position.x].GetCell());

		terrain[y][x].GetCell().DecreaceEnergy(terrain[y][x].GetCell().ReprodutionCost());
		unsigned short hlph = terrain[y][x].GetCell().Energy() / 2;
		terrain[y][x].GetCell().DecreaceEnergy(hlph);
		terrain[new_position.y][new_position.x].GetCell().IncreaceEnergy(hlph);
		cells[cells_count] = &terrain[new_position.y][new_position.x].GetCell();
		cells[cells_count]->SetPosition(new_position.x, new_position.y);

		size_t cost = 0;
		for (const Gen& gen : cells[cells_count]->GetGenome().data)
		{
			cost += CREATION_COST.at(gen.trigger);
		}
		cells[cells_count]->ReproductionCost(100 + cost);

		cells_count++;
	}
}

void Environment::Update()
{
	for (size_t index = 0; index < cells.size(); index++)
	{
		if (!cells[index]) break;

		size_t x = cells[index]->GetX();
		size_t y = cells[index]->GetY();
		if (cells[index]->IsDead())
			// remove cell if it is dead
		{
			CellDie(index, x, y);
		}
		else
		{
			cells[index]->Tic();

			Gen& gen = cells[index]->GetGenome().Read();
			cells[index]->DecreaceEnergy(MAINTENANACE_COST.at(gen.trigger));
			switch (gen.trigger)
			{
			case Trigger::Separate:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Birth:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Carnivorous:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Mineraleon:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Photosyntesis:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Stay:
				Separationing(gen.args, x, y);
				break;
			case Trigger::Turn:
				Separationing(gen.args, x, y);
				break;
			}
		}
	}
}

bool operator == (const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

Position Environment::GetViewedPosition(view_side view, Position init)
{
	auto maxX = ENVIRONMENT_SIZE_X - 1;
	auto maxY = ENVIRONMENT_SIZE_Y - 1;

	Position viewed_position = init;
	switch (view)
	{
	case view_side::left:
		if (init.x > 0)
		{
			viewed_position.x--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
		}
		break;
	case view_side::right:
		if (init.x < maxX)
		{
			viewed_position.x++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
		}
		break;
	case view_side::bottom:
		if (init.y > 0)
		{
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.y = maxY;
		}
		break;
	case view_side::top:
		if (init.y < maxY)
		{
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.y = 0;
		}
		break;
	case view_side::left_bottom:
		if (init.x > 0 && init.y < maxY)
		{
			viewed_position.x--;
			viewed_position.y++;
		}
		else if (init.y < maxY && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT && init.x > 0)
		{
			viewed_position.x--;
			viewed_position.y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y = 0;
		}
		break;
	case view_side::left_top:
		if (init.x > 0 && init.y > 0)
		{
			viewed_position.x--;
			viewed_position.y--;
		}
		else if (init.y > 0 && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT && init.x > 0)
		{
			viewed_position.x--;
			viewed_position.y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y = maxY;
		}
		break;
	case view_side::right_bottom:
		if (init.y < maxY && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y++;
		}
		else if (init.y < maxY && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y = 0;
		}
		break;
	case view_side::right_top:
		if (init.y > 0 && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y--;
		}
		else if (init.y > 0 && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y = maxY;
		}
		break;
	}
	return viewed_position;
}

Position Environment::GetInvertedViewedPosition(view_side view, Position init)
{
	switch (view)
	{
	case view_side::top:
		return GetViewedPosition(view_side::bottom, init);
	case view_side::right_top:
		return GetViewedPosition(view_side::left_bottom, init);
	case view_side::right:
		return GetViewedPosition(view_side::left, init);
	case view_side::right_bottom:
		return GetViewedPosition(view_side::left_top, init);
	case view_side::bottom:
		return GetViewedPosition(view_side::top, init);
	case view_side::left_bottom:
		return GetViewedPosition(view_side::right_top, init);
	case view_side::left:
		return GetViewedPosition(view_side::right, init);
	case view_side::left_top:
		return GetViewedPosition(view_side::right_bottom, init);
	default:
		return GetViewedPosition(view_side::top, init);
	}
}
void Environment::Birthing(int args, size_t x, size_t y)
{
	if (terrain[y][x].GetCell().Energy() < (terrain[y][x].GetCell().ReprodutionCost() / 2))
		return;

	Position new_position = GetInvertedViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[new_position.y][new_position.x].SetCell();
		terrain[y][x].GetCell().DecreaceEnergy(terrain[y][x].GetCell().ReprodutionCost() / 2);
		terrain[y][x].GetCell().Reproduction(terrain[new_position.y][new_position.x].GetCell());
		cells[cells_count] = &terrain[new_position.y][new_position.x].GetCell();
		cells[cells_count]->SetPosition(new_position.x, new_position.y);

		size_t cost = 0;
		for (const Gen& gen : cells[cells_count]->GetGenome().data)
		{
			cost += CREATION_COST.at(gen.trigger);
		}
		cells[cells_count]->ReproductionCost(100 + cost);

		cells_count++;
	}
}
void Environment::Carnivorousing(int args, size_t x, size_t y)
{
	terrain[y][x].GetCell().RationLevel(5, -1, -1);
	Position viewed_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (viewed_position == Position{ x, y })
		return;

	auto& viewed_point = terrain[viewed_position.y][viewed_position.x];

	if (viewed_point.ContainsCell())
	{
		if (!viewed_point.GetCell().IsFriendly(terrain[y][x].GetCell()))
		{
			if (!viewed_point.GetCell().Defencing(terrain[y][x].GetCell().Attack()))
			{
				CellDie(x, y);
				terrain[y][x].GetCell().AttackUp();
				auto e = viewed_point.GetCell().Energy();
				if (e > MAX_MEAT_TO_EAT)
					terrain[y][x].GetCell().IncreaceEnergy(MAX_MEAT_TO_EAT);
				else
					terrain[y][x].GetCell().IncreaceEnergy(e);
			}
		}
	}
}
void Environment::Mineraling(int args, size_t x, size_t y)
{
	terrain[y][x].GetCell().RationLevel(-1, -1, 5);
	auto e = terrain[y][x].GetFood().Eat(MAX_MINERALS_TO_EAT);
	if (e)
	{
		terrain[y][x].GetCell().IncreaceEnergy(e);
	}
}
void Environment::Moving(int args, size_t x, size_t y)
{
	Position new_position = GetViewedPosition(terrain[y][x].GetCell().GetView(), { x, y });

	if (new_position == Position{ x, y })
		return;

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		size_t index;
		for (index = 0; index < cells.size(); index++)
		{
			if (cells[index]->GetX() == x &&
				cells[index]->GetY() == y)
			{
				cells[index] = &terrain[new_position.y][new_position.x].GetCell();
				break;
			}
		}
		terrain[y][x].GetCell().SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetCell(terrain[y][x].GetCell());

		terrain[y][x].DelCell();
	}
}
void Environment::Photosynthesing(int args, size_t x, size_t y)
{
	terrain[y][x].GetCell().RationLevel(-1, 5, -1);
	terrain[y][x].GetCell().IncreaceEnergy(LIGHT_POWER);
}
void Environment::Staying(int args, size_t x, size_t y) {}
void Environment::Turning(int args, size_t x, size_t y)
{
	unsigned short old_side = static_cast<unsigned short>(terrain[y][x].GetCell().GetView());
	view_side new_side;
	if (args % 2)
	{
		new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
	}
	else
	{
		new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
	}
	terrain[y][x].GetCell().SetView(new_side);
}