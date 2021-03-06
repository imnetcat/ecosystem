#include "environment.h"
#include "config.h"
#include "Water.h"

Environment::Environment()
	: max_generation(1)
{
	size_t index = 0;
	Random random;

	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			// put food
			auto rand = random.Generate(101);
			if (rand < 30)
			{
				terrain[y][x].SetFood(100);
			}
			// put first entities
			if (rand < 1 && entities.size() < CELL_START_COUNT)
			{
				terrain[y][x].SetEntity(entities.Add({ 
					x, y,
					view_side::top,
					100,
					100,
					0.01,
					0.01,
					Genome()
				}));
			}

			index++;
		}
	}
}

EntitiesIterator Environment::EntityDie(EntitiesIterator entity_iterator)
{
	auto x = entity_iterator->GetX();
	auto y = entity_iterator->GetY();
	terrain[y][x].SetFood(terrain[y][x].GetFood() + entity_iterator->Energy() + 100);
	terrain[y][x].DelEntity();
	return entities.Del(entity_iterator);
}

void Environment::Update()
{
	auto entity = entities.begin();
	while (entity != entities.end())
	{
		if (entity->IsDead())
			// remove cell if it is dead
		{
			entity = EntityDie(entity);
			continue;
		}
		else
		{
			entity->Tic();

			Gen gen = entity->GetGenome().Read();
			entity->DecreaceEnergy(MAINTENANACE_COST.at(gen.trigger));
			switch (gen.trigger)
			{
			case Trigger::Stay:
				break;
			case Trigger::Separate:
				Separationing(gen.args, entity);
				break;
			case Trigger::Birth:
				Birthing(gen.args, entity);
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
			case Trigger::Turn:
				Turning(gen.args, entity);
				break;
			}
		}
		entity++;
	}
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

view_side Environment::GetViewSide(unsigned __int8 arg)
{
	// Define view side
	/*
		0 1 2
		7   3
		6 5	4
	*/
	switch (arg % 8)
	{
	case 0:
		return view_side::left_top;
	case 1:
		return view_side::top;
	case 2:
		return view_side::right_top;
	case 3:
		return view_side::right;
	case 4:
		return view_side::right_bottom;
	case 5:
		return view_side::bottom;
	case 6:
		return view_side::left_bottom;
	case 7:
		return view_side::left;
	default:
		return view_side::left;
	}
}

Coefficient Environment::SuccessRule(EntitiesIterator entity)
{
	unsigned int success_border = MAX_ENERGY * (double(entity->Age()) / (entity->MaxAge()));
	return (entity->Energy() > success_border) ? Coefficient::enlarge :
		(entity->Energy() > (success_border / 2) ? Coefficient::unchanged : Coefficient::reduce);
}

EntitiesIterator Environment::Reproduction(EntitiesIterator parent_entity, size_t x, size_t y, view_side view)
{
	Coefficient coef = SuccessRule(parent_entity);
	
	Genome new_genom = parent_entity->GetGenome().Replicate(coef);

	unsigned short new_max_age = parent_entity->MaxAge();

	switch (coef)
	{
	case Coefficient::enlarge:
		if (new_max_age < 1000) new_max_age++;
		break;
	case Coefficient::reduce:
		if (new_max_age > 0) new_max_age--;
		break;
	case Coefficient::unchanged:
	default:
		break;
	}
	
	parent_entity->DecreaceEnergy(parent_entity->ReproductionCost());

	terrain[y][x].SetEntity(entities.Add({
		x, y,
		view,
		100,
		new_max_age,
		parent_entity->Defence(),
		parent_entity->Attack(),
		new_genom
	}));

	if (new_genom.Generation() > max_generation)
		max_generation = new_genom.Generation();

	return terrain[y][x].GetEntity();
}

void Environment::Birthing(unsigned __int8 args, EntitiesIterator entity)
{
	if (entity->Energy() < (entity->ReproductionCost() / 2))
		return;

	Position new_position = GetViewedPosition(GetViewSide(args), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		Reproduction(entity, new_position.x, new_position.y, view_side::top);
	}
}

void Environment::Separationing(unsigned __int8 args, EntitiesIterator entity)
{
	if (entity->Energy() < entity->ReproductionCost())
		return;

	Position new_position = GetViewedPosition(GetViewSide(args), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		EntitiesIterator nEntity = Reproduction(entity, new_position.x, new_position.y, view_side::top);

		unsigned short hlph = entity->Energy() / 2;
		entity->DecreaceEnergy(hlph);
		nEntity->IncreaceEnergy(hlph);
	}
}

void Environment::Carnivorousing(EntitiesIterator entity)
{
	Position viewed_position = GetViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	auto& viewed_point = terrain[viewed_position.y][viewed_position.x];

	if (viewed_point.ContainsEntity())
	{
		auto lhs_genom = viewed_point.GetEntity()->GetGenome().Data();
		auto rhs_genom = viewed_point.GetEntity()->GetGenome().Data();

		if (lhs_genom == rhs_genom)
			return;

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
	}
}
void Environment::Mineraling(EntitiesIterator entity)
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
void Environment::Moving(EntitiesIterator entity)
{
	Position new_position = GetViewedPosition(entity->GetView(), entity->GetX(), entity->GetY());

	if (terrain[new_position.y][new_position.x].IsWalkable())
	{
		terrain[entity->GetY()][entity->GetX()].DelEntity();
		entity->SetPosition(new_position.x, new_position.y);
		terrain[new_position.y][new_position.x].SetEntity(entity);
	}
}
void Environment::Photosynthesing(EntitiesIterator entity)
{
	entity->IncreaceEnergy(LIGHT_POWER);
}
void Environment::Turning(unsigned __int8 args, EntitiesIterator entity)
{
	// Set up new side of view for entity
	entity->SetView(GetViewSide(args));
}
