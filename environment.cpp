#include "environment.h"
#include "config.h"

Environment::Environment(
	unsigned int width,
	unsigned int height,
	unsigned short light_power,
	double light_coef,
	unsigned short max_organic_to_eat,
	unsigned short max_entities_to_eat,
	unsigned short max_energy,
	unsigned short max_hp,
	unsigned short entities_start_count
)
	: max_generation(1)
	, width(width)
	, height(height)
	, terrain(nullptr)
	, entities((size_t)height * width)
	, organic((size_t)height * width)
	, light_power(light_power)
	, light_coef(light_coef)
	, max_organic_to_eat(max_organic_to_eat)
	, max_entities_to_eat(max_entities_to_eat)
	, max_energy(max_energy)
	, max_hp(max_hp)
{
	// Allocate 2d array for world map
	terrain = new TerrainCell * [height];
	for (size_t y = 0; y < height; y++)
	{
		terrain[y] = new TerrainCell[width];
	}

	Random random;
	// Put first entities
	while (entities_start_count--)
	{
		auto x = random.Generate(width);
		auto y = random.Generate(height);
		terrain[y][x].SetEntity(entities.Add({
			x, y,
			view_side::top,
			max_hp,
			100,
			max_energy,
			100,
			0.01,
			0.01,
			Genome()
		}));
	}
}
Environment::~Environment()
{
	for (size_t y = 0; y < height; y++)
	{
		delete[] terrain[y];
	}
	delete[] terrain;
}

EntitiesIterator Environment::EntityDie(EntitiesIterator entity_iterator)
{
	auto x = entity_iterator->GetX();
	auto y = entity_iterator->GetY();
	if (terrain[y][x].IsContainsOrganic())
	{
		terrain[y][x].AddOrganic(entity_iterator->Energy() + 100ull);
	}
	else
	{
		terrain[y][x].SetOrganic(organic.Add({ x, y, entity_iterator->Energy() + 100ull }));
	}
	terrain[y][x].DelEntity();
	if (observed_entity)
	{
		if (observed_entity->GetX() == entity_iterator->GetX() &&
			observed_entity->GetY() == entity_iterator->GetY())
		{
			observed_entity = nullptr;
		}
	}
	return entities.Del(entity_iterator);
}

void Environment::Update()
{
	// Update gravity
	auto object = organic.begin();
	while (object != organic.end())
	{
		auto x = object->GetX();
		auto y = object->GetY();
		if (y == (height - 1))
		{
			object++;
			continue;
		}
		if (terrain[y + 1][x].IsContainsOrganic())
		{
			object++;
			continue;
		}
		terrain[y][x].DelOrganic();
		y++;
		object->SetY(y);
		terrain[y][x].SetOrganic(object);
		object++;
	}

	// Update entities
	auto entity = entities.begin();
	while (entity != entities.end())
	{
		if (entity->IsDead())
			// remove entity if it is dead
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
				EatOrganic(entity);
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
	auto maxX = width - 1;
	auto maxY = height - 1;

	switch (view)
	{
	case view_side::left:
		if (x > 0)
		{
			x--;
		}
		else
		{
			x = maxX;
		}
		break;
	case view_side::right:
		if (x < maxX)
		{
			x++;
		}
		else
		{
			x = 0;
		}
		break;
	case view_side::bottom:
		if (y > 0)
		{
			y--;
		}
		break;
	case view_side::top:
		if (y < maxY)
		{
			y++;
		}
		break;
	case view_side::left_bottom:
		if (x > 0 && y < maxY)
		{
			x--;
			y++;
		}
		else if (y < maxY)
		{
			x = maxX;
			y++;
		}
		break;
	case view_side::left_top:
		if (x > 0 && y > 0)
		{
			x--;
			y--;
		}
		else if (y > 0)
		{
			x = maxX;
			y--;
		}
		break;
	case view_side::right_bottom:
		if (y < maxY && x < maxX)
		{
			x++;
			y++;
		}
		else if (y < maxY)
		{
			x = 0;
			y++;
		}
		break;
	case view_side::right_top:
		if (y > 0 && x < maxX)
		{
			x++;
			y--;
		}
		else if (y > 0)
		{
			x = 0;
			y--;
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
	unsigned int success_border = max_energy * (double(entity->Age()) / (entity->MaxAge()));
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
		max_hp,
		100,
		max_energy,
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
			if (e > max_entities_to_eat)
				entity->IncreaceEnergy(max_entities_to_eat);
			else
				entity->IncreaceEnergy(e);
		}
	}
}
void Environment::EatOrganic(EntitiesIterator entity)
{
	if (!terrain[entity->GetY()][entity->GetX()].IsContainsOrganic())
	{
		return;
	}

	auto o = terrain[entity->GetY()][entity->GetX()].GetOrganic();
	auto energy = o->Energy();

	if (energy >= max_organic_to_eat)
	{
		entity->IncreaceEnergy(max_organic_to_eat);
		o->Decreace(max_organic_to_eat);
		if (!o->Energy())
		{
			organic.Del(o);
			terrain[entity->GetY()][entity->GetX()].DelOrganic();
		}
	}
	else
	{
		entity->IncreaceEnergy(energy);
		organic.Del(o);
		terrain[entity->GetY()][entity->GetX()].DelOrganic();
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
	entity->IncreaceEnergy(
			(((height - (double)entity->GetY()) / height) * light_coef) * light_power
	);
}
void Environment::Turning(unsigned __int8 args, EntitiesIterator entity)
{
	// Set up new side of view for entity
	entity->SetView(GetViewSide(args));
}
