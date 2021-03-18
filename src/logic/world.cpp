#include "world.h"
using namespace Ecosystem::Logic;

World::World(
	unsigned int width,
	unsigned int height,
	unsigned short light_power,
	unsigned short max_organic_to_eat,
	unsigned short max_entities_to_eat,
	unsigned short max_energy
)
	: max_generation(1)
	, width(width)
	, height(height)
	, terrain(nullptr)
	, entities((size_t)height* width)
	, organic((size_t)height* width)
	, light_power(light_power)
	, max_organic_to_eat(max_organic_to_eat)
	, max_entities_to_eat(max_entities_to_eat)
	, max_energy(max_energy)
{
	// Allocate 2d array for world map
	terrain = new cell * [height];
	for (size_t y = 0; y < height; y++)
	{
		terrain[y] = new cell[width];
	}
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			terrain[y][x].Init(light_power, x, y);
		}
	}


	// Put first entity
	auto x = width / 2;
	auto y = height / 8;
	terrain[y][x].SetEntity(entities.get({
		x, y,
		100,
		max_energy,
		100,
		0.01,
		0.01,
		Genome(
			random::Generate(),
			8,
			random::Generate(std::numeric_limits<unsigned __int8>().max()),
			1,
			10
		)
	}));
}

World::~World()
{
	for (size_t y = 0; y < height; y++)
	{
		delete[] terrain[y];
	}
	delete[] terrain;
}

void World::Reload()
{
	// Remove all organics and entities
	entities.clear();
	organic.clear();

	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			terrain[y][x].DelEntity();
			terrain[y][x].DelOrganic();
		}
	}

	// Put first entity
	auto x = width / 2;
	auto y = height / 8;
	terrain[y][x].SetEntity(entities.get({
		x, y,
		100,
		max_energy,
		100,
		0.01,
		0.01,
		Genome(
			random::Generate(),
			8,
			random::Generate(std::numeric_limits<unsigned __int8>().max()),
			1,
			10
		)
	}));
}

EntitiesIterator World::EntityDie(EntitiesIterator entity_iterator)
{
	auto x = entity_iterator->x();
	auto y = entity_iterator->y();
	if (terrain[y][x].ContainsOrganic())
	{
		terrain[y][x].AddOrganic(entity_iterator->Energy() + 100ull);
	}
	else
	{
		terrain[y][x].SetOrganic(organic.get({ x, y, entity_iterator->Energy() + 100ull }));
	}
	terrain[y][x].DelEntity();
	return entities.free(entity_iterator);
}

void World::Update()
{
	// Update gravity
	auto object = organic.begin();
	while (object != organic.end())
	{
		auto x = object->x();
		auto y = object->y();
		if (y == (height - 1))
		{
			object++;
			continue;
		}
		if (terrain[y + 1][x].ContainsOrganic())
		{
			object++;
			continue;
		}
		terrain[y][x].DelOrganic();
		y++;
		object->y(y);
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
			case Operation::Stay:
				break;
			case Operation::Separate:
				Separationing(gen.args, entity);
				break;
			case Operation::Birth:
				Birthing(gen.args, entity);
				break;
			case Operation::Carnivorous:
				Carnivorousing(gen.args, entity);
				break;
			case Operation::EatOrganic:
				EatOrganic(entity);
				break;
			case Operation::Photosyntesis:
				Photosynthesing(entity);
				break;
			case Operation::Move:
				Moving(gen.args, entity);
				break;
			}
		}
		entity++;
	}
}

bool World::GetViewPos(unsigned __int8 arg, position& pos)
{
	auto maxX = width - 1;
	auto maxY = height - 1;

	/*
		Define view side

			0 1 2
			7   3
			6 5	4
	*/
	switch (arg % 8)
	{
	case 0:
		if (pos.x() > 0 && pos.y() > 0)
		{
			pos.x(pos.x() - 1);
			pos.y(pos.y() - 1);
		}
		else if (pos.y() > 0)
		{
			pos.x(maxX);
			pos.y(pos.y() - 1);
		}
		else
		{
			return false;
		}
		break;
	case 1:
		if (pos.y() < maxY)
		{
			pos.y(pos.y() + 1);
		}
		else
		{
			return false;
		}
		break;
	case 2:
		if (pos.y() > 0 && pos.x() < maxX)
		{
			pos.x(pos.x() + 1);
			pos.y(pos.y() - 1);
		}
		else if (pos.y() > 0)
		{
			pos.x(0);
			pos.y(pos.y() - 1);
		}
		else
		{
			return false;
		}
		break;
	case 3:
		if (pos.x() < maxX)
		{
			pos.x(pos.x() + 1);
		}
		else
		{
			pos.x(0);
		}
		break;
	case 4:
		if (pos.y() < maxY && pos.x() < maxX)
		{
			pos.x(pos.x() + 1);
			pos.y(pos.y() + 1);
		}
		else if (pos.y() < maxY)
		{
			pos.x(0);
			pos.y(pos.y() + 1);
		}
		else
		{
			return false;
		}
		break;
	case 5:
		if (pos.y() > 0)
		{
			pos.y(pos.y() - 1);
		}
		else
		{
			return false;
		}
		break;
	case 6:
		if (pos.x() > 0 && pos.y() < maxY)
		{
			pos.x(pos.x() - 1);
			pos.y(pos.y() + 1);
		}
		else if (pos.y() < maxY)
		{
			pos.x(maxX);
			pos.y(pos.y() + 1);
		}
		else
		{
			return false;
		}
		break;
	case 7:
		if (pos.x() > 0)
		{
			pos.x(pos.x() - 1);
		}
		else
		{
			pos.x(maxX);
		}
		break;
	default:
		return false;
	}

	return true;
}

Coefficient World::SuccessRule(pool<Entity>::const_iterator entity) const
{
	unsigned int success_border = entity->MaxEnergy() * (double(entity->Age()) / (entity->MaxAge()));
	return (entity->Energy() > success_border) ? Coefficient::enlarge :
		(entity->Energy() > (success_border / 2) ? Coefficient::unchanged : Coefficient::reduce);
}

EntitiesIterator World::Reproduction(EntitiesIterator parent_entity, const position& pos)
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

	terrain[pos.y()][pos.x()].SetEntity(entities.get({
		pos.x(), pos.y(),
		100,
		parent_entity->MaxEnergy(),
		new_max_age,
		parent_entity->Defence(),
		parent_entity->Attack(),
		new_genom
	}));

	if (new_genom.Generation() > max_generation)
		max_generation = new_genom.Generation();

	return terrain[pos.y()][pos.x()].GetEntity();
}

void World::Birthing(unsigned __int8 args, EntitiesIterator entity)
{
	if (entity->Energy() < (entity->ReproductionCost() / 2))
		return;

	position new_position = { entity->x(), entity->y() };
	if (!GetViewPos(args, new_position))
		return;

	if (!terrain[new_position.y()][new_position.x()].ContainsEntity())
	{
		Reproduction(entity, new_position);
	}
}

void World::Separationing(unsigned __int8 args, EntitiesIterator entity)
{
	if (entity->Energy() < entity->ReproductionCost())
		return;

	position new_position = { entity->x(), entity->y() };
	if (!GetViewPos(args, new_position))
		return;

	if (!terrain[new_position.y()][new_position.x()].ContainsEntity())
	{
		EntitiesIterator nEntity = Reproduction(entity, new_position);

		unsigned short hlph = entity->Energy() / 2;
		entity->DecreaceEnergy(hlph);
		nEntity->IncreaceEnergy(hlph);
	}
}

void World::Carnivorousing(unsigned __int8 args, EntitiesIterator entity)
{
	position viewed_position = { entity->x(), entity->y() };
	if (!GetViewPos(args, viewed_position))
		return;

	const auto& viewed_point = terrain[viewed_position.y()][viewed_position.x()];

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

void World::EatOrganic(EntitiesIterator entity)
{
	if (!terrain[entity->y()][entity->x()].ContainsOrganic())
	{
		return;
	}

	auto o = terrain[entity->y()][entity->x()].GetOrganic();
	auto energy = o->Energy();

	if (energy >= max_organic_to_eat)
	{
		entity->IncreaceEnergy(max_organic_to_eat);
		o->Decreace(max_organic_to_eat);
		if (!o->Energy())
		{
			organic.free(o);
			terrain[entity->y()][entity->x()].DelOrganic();
		}
	}
	else
	{
		entity->IncreaceEnergy(energy);
		organic.free(o);
		terrain[entity->y()][entity->x()].DelOrganic();
	}
}
void World::Moving(unsigned __int8 args, EntitiesIterator entity)
{
	position new_position = { entity->x(), entity->y() };
	if (!GetViewPos(args, new_position))
		return;

	if (!terrain[new_position.y()][new_position.x()].ContainsEntity())
	{
		terrain[entity->y()][entity->x()].DelEntity();
		entity->SetPosition(new_position.x(), new_position.y());
		terrain[new_position.y()][new_position.x()].SetEntity(entity);
	}
}
void World::Photosynthesing(EntitiesIterator entity)
{
	entity->IncreaceEnergy(
		terrain[entity->y()][entity->x()].LightPower()
	);
}

size_t World::MaxGeneration() const
{
	return max_generation;
}
cell** const World::Terrain() const
{
	return terrain;
}
const pool<Entity>& World::Entities() const
{
	return entities;
}
const pool<Organic>& World::Organics() const
{
	return organic;
}

unsigned int World::Width() const
{
	return width;
}
unsigned int World::Height() const
{
	return height;
}