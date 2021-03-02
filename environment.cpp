#include "environment.h"
#include "config.h"
#include "Water.h"

Environment::Environment()
	: shift_count(0)
	, entities_count(0)
	, max_generation(1)
{
	size_t index = 0;
	srand(time(0));
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			// put food
			if (((rand() % 100) < 30))
			{
				terrain[y][x].SetFood(100);
			}

			// put first entities
			if (((rand() % 100) < 30) && entities_count < CELL_START_COUNT)
			{
				terrain[y][x].SetEntity(entities.begin() + entities_count);
				entities[entities_count].SetPosition(x, y);
				entities[entities_count].Energy(100);
				entities[entities_count].MaxAge(100);
				entities[entities_count].Defence(0.01);
				entities[entities_count].Hp(MAX_HP);
				entities[entities_count].Attack(0.01);
				entities[entities_count].SetGenome(Genome());

				size_t cost = 0;
				for (const Gen& gen : entities[entities_count].GetGenome().data)
				{
					cost += CREATION_COST.at(gen.trigger);
				}
				entities[entities_count].ReproductionCost(100 + cost);

				entities_count++;
			}

			index++;
		}
	}
}

void Environment::EntityDie(EntityIterator entity_iterator)
{
	if (!entities_count)
	{
		return;
	}

	auto x = entity_iterator->GetX();
	auto y = entity_iterator->GetY();
	terrain[y][x].SetFood((size_t)entity_iterator->Energy() + 100);
	terrain[y][x].DelEntity();
	shift_count++;
}

void Environment::Update()
{
	size_t i = 0;
	shift_count = 0;
	while (i < entities_count)
	{
		auto entity = entities.begin() + i;
		i++;
		size_t x = entity->GetX();
		size_t y = entity->GetY();
		if (entity->IsDead())
			// remove cell if it is dead
		{
			EntityDie(entity);
			continue;
		}
		else
		{
			entity->Tic();

			Gen& gen = entity->GetGenome().Read();
			entity->DecreaceEnergy(MAINTENANACE_COST.at(gen.trigger));
			switch (gen.trigger)
			{
			case Trigger::Separate:
				Separationing(entity);
				break;
			case Trigger::Birth:
				Birthing(entity);
				break;
			case Trigger::Carnivorous:
				Carnivorousing(entity);
				break;
			case Trigger::Mineraleon:
				Mineraling(entity);
				break;
			case Trigger::Photosyntesis:
				Photosynthesing(entity);
				break;
			case Trigger::Stay:
				Staying();
				break;
			case Trigger::Turn:
				Turning(gen.args, entity);
				break;
			}
		}

		if (shift_count)
		{
			entities[i - shift_count] = entities[i];
		}
	}

	entities_count -= shift_count;
}

bool operator == (const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

Position Environment::GetViewedPosition(view_side view, size_t x, size_t y)
{
	auto maxX = ENVIRONMENT_SIZE_X - 1;
	auto maxY = ENVIRONMENT_SIZE_Y - 1;

	switch (view)
	{
	case view_side::left:
		if (x > 0)
		{
			x--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = maxX;
		}
		break;
	case view_side::right:
		if (x < maxX)
		{
			x++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = 0;
		}
		break;
	case view_side::bottom:
		if (y > 0)
		{
			y--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			y = maxY;
		}
		break;
	case view_side::top:
		if (y < maxY)
		{
			y++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			y = 0;
		}
		break;
	case view_side::left_bottom:
		if (x > 0 && y < maxY)
		{
			x--;
			y++;
		}
		else if (y < maxY && LOOPED_ENVIRONMENT)
		{
			x = maxX;
			y++;
		}
		else if (LOOPED_ENVIRONMENT && x > 0)
		{
			x--;
			y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = maxX;
			y = 0;
		}
		break;
	case view_side::left_top:
		if (x > 0 && y > 0)
		{
			x--;
			y--;
		}
		else if (y > 0 && LOOPED_ENVIRONMENT)
		{
			x = maxX;
			y--;
		}
		else if (LOOPED_ENVIRONMENT && x > 0)
		{
			x--;
			y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = maxX;
			y = maxY;
		}
		break;
	case view_side::right_bottom:
		if (y < maxY && x < maxX)
		{
			x++;
			y++;
		}
		else if (y < maxY && LOOPED_ENVIRONMENT)
		{
			x = 0;
			y++;
		}
		else if (LOOPED_ENVIRONMENT && x < maxX)
		{
			x++;
			y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = 0;
			y = 0;
		}
		break;
	case view_side::right_top:
		if (y > 0 && x < maxX)
		{
			x++;
			y--;
		}
		else if (y > 0 && LOOPED_ENVIRONMENT)
		{
			x = 0;
			y--;
		}
		else if (LOOPED_ENVIRONMENT && x < maxX)
		{
			x++;
			y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			x = 0;
			y = maxY;
		}
		break;
	}
	return { x, y };
}

Position Environment::GetInvertedViewedPosition(view_side view, size_t x, size_t y)
{
	switch (view)
	{
	case view_side::top:
		return GetViewedPosition(view_side::bottom, x, y);
	case view_side::right_top:
		return GetViewedPosition(view_side::left_bottom, x, y);
	case view_side::right:
		return GetViewedPosition(view_side::left, x, y);
	case view_side::right_bottom:
		return GetViewedPosition(view_side::left_top, x, y);
	case view_side::bottom:
		return GetViewedPosition(view_side::top, x, y);
	case view_side::left_bottom:
		return GetViewedPosition(view_side::right_top, x, y);
	case view_side::left:
		return GetViewedPosition(view_side::right, x, y);
	case view_side::left_top:
		return GetViewedPosition(view_side::right_bottom, x, y);
	default:
		return GetViewedPosition(view_side::top, x, y);
	}
}


Environment::Success Environment::SuccessRule(EntityIterator entity)
{
	unsigned int success_border = MAX_ENERGY * (double(entity->Age()) / (entity->MaxAge()));
	return (entity->Energy() > success_border) ? Success::good :
		(entity->Energy() > (success_border / 2) ? Success::normal : Success::fail);
}

size_t Environment::Reproduction(EntityIterator parent_entity, EntityIterator new_entity)
{
	Success isSuccess = SuccessRule(parent_entity);
	double newMutationChanceK = 0;
	switch (isSuccess)
	{
	case Success::fail:
		newMutationChanceK = 0.01;
		break;
	case Success::good:
		newMutationChanceK = -0.01;
		break;
	}
	Genome new_genom = parent_entity->GetGenome().Replicate(newMutationChanceK);

	short max_age_koef = 0;
	switch (isSuccess)
	{
	case Success::fail:
		max_age_koef = -1;
		break;
	case Success::good:
		max_age_koef = 1;
		break;
	}

	unsigned short new_max_age = parent_entity->MaxAge() + max_age_koef;
	if (new_max_age > 1000) new_max_age = 1000;
	if (new_max_age < 2) new_max_age = 2;

	new_entity->Energy(100);
	new_entity->Age(0);
	new_entity->MaxAge(new_max_age);
	new_entity->ReproductionCost(parent_entity->ReproductionCost());
	new_entity->Defence(parent_entity->Defence());
	new_entity->Attack(parent_entity->Attack());
	new_entity->SetGenome(new_genom);
	new_entity->Hp(MAX_HP);

	return new_genom.generation;
}
void Environment::Birthing(EntityIterator entity)
{/*
	if (entity->Energy() < (entity->ReproductionCost() / 2))
		return;

	Position new_position = GetInvertedViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		if (Reproduction(entity, entities.begin() + entities_count) > max_generation)
			max_generation++;

		entity->DecreaceEnergy(entity->ReproductionCost());
		entities[entities_count].SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetEntity(entities.begin() + entities_count);

		size_t cost = 0;
		for (const Gen& gen : entities[entities_count].GetGenome().data)
		{
			cost += CREATION_COST.at(gen.trigger);
		}
		entities[entities_count].ReproductionCost(100 + cost);

		entities_count++;
	}*/
}

void Environment::Separationing(EntityIterator entity)
{/*
	if (entity->Energy() < entity->ReproductionCost())
		return;

	Position new_position = GetInvertedViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		if (Reproduction(entity, entities.begin() + entities_count) > max_generation)
			max_generation++;

		entity->DecreaceEnergy(entity->ReproductionCost());
		unsigned short hlph = entity->Energy() / 2;
		entity->DecreaceEnergy(hlph);
		entities[entities_count].IncreaceEnergy(hlph);
		entities[entities_count].SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetEntity(entities.begin() + entities_count);

		size_t cost = 0;
		for (const Gen& gen : entities[entities_count].GetGenome().data)
		{
			cost += CREATION_COST.at(gen.trigger);
		}
		entities[entities_count].ReproductionCost(100 + cost);

		entities_count++;
	}*/
}

void Environment::Carnivorousing(EntityIterator entity)
{/*
	Position viewed_position = GetViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	auto& viewed_point = terrain[viewed_position.y][viewed_position.x];

	if (viewed_point.ContainsEntity())
	{
		const unsigned short BORDER = 8;
		size_t count_of_non_equal = 0;
		auto lhs_genom = viewed_point.GetEntity()->GetGenome().data;
		auto rhs_genom = viewed_point.GetEntity()->GetGenome().data;

		if (lhs_genom.size() != rhs_genom.size())
			return;

		for (size_t index = 0; index < lhs_genom.size(); index++)
		{
			if (lhs_genom[index].trigger != rhs_genom[index].trigger &&
				lhs_genom[index].args != rhs_genom[index].args)
				count_of_non_equal++;

			if (count_of_non_equal > BORDER)
				return;
		}

		if (!viewed_point.GetEntity()->Defencing(entity->Attack()))
		{
			EntityDie(viewed_point.GetEntity());
			entity->AttackUp();
			auto e = viewed_point.GetEntity()->Energy();
			if (e > MAX_MEAT_TO_EAT)
				entity->IncreaceEnergy(MAX_MEAT_TO_EAT);
			else
				entity->IncreaceEnergy(e);
		}
	}*/
}
void Environment::Mineraling(EntityIterator entity)
{
	auto e = terrain[entity->GetY()][entity->GetX()].GetFood();
	if (e >= MAX_MINERALS_TO_EAT)
	{
		entity->IncreaceEnergy(MAX_MINERALS_TO_EAT);
		terrain[entity->GetY()][entity->GetX()].SetFood(e - MAX_MINERALS_TO_EAT);
	}
	else
	{
		entity->IncreaceEnergy(e);
		terrain[entity->GetY()][entity->GetX()].DelFood();
	}
}
void Environment::Moving(EntityIterator entity)
{
	Position new_position = GetViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		//terrain[entity->GetY()][entity->GetX()].DelEntity();
		//entity->SetPosition(new_position.x, new_position.y);
		//terrain[new_position.y][new_position.x].SetEntity(entity);
	}
}
void Environment::Photosynthesing(EntityIterator entity)
{
	entity->IncreaceEnergy(LIGHT_POWER);
}
void Environment::Staying() {}
void Environment::Turning(int args, EntityIterator entity)
{
	unsigned short old_side = static_cast<unsigned short>(entity->GetView());
	view_side new_side;
	if (args % 2)
	{
		new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
	}
	else
	{
		new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
	}
	entity->SetView(new_side);
}