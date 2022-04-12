#include "Entity.h"
using namespace ecosystem::logic;

Entity::Entity()
	:
	position(0, 0),
	age(0),
	max_age(0),
	energy(0),
	max_energy(0)
{}

Entity::Entity(
	int x,
	int y,
	unsigned short energy,
	unsigned short max_energy,
	Genome g,
	unsigned short max_age
)
	:
	position(x, y),
	age(0),
	max_age(max_age),
	energy(energy),
	max_energy(max_energy),
	genom(g)
{}

unsigned short Entity::Age() const
{
	return age;
}
unsigned short Entity::MaxAge() const
{
	return max_age;
}

void Entity::IncreaceEnergy(unsigned short value)
{
	if (energy + value < max_energy)
		energy += value;
	else
		energy = max_energy;
}

void Entity::DecreaceEnergy(unsigned short value)
{
	if (energy > value)
	{
		energy -= value;
		return;
	}

	value -= energy;
	energy = 0;
}

unsigned short Entity::Energy() const
{
	return energy;
}
unsigned short Entity::MaxEnergy() const
{
	return max_energy;
}

bool Entity::IsDead() const
{
	return !energy || age > max_age;
}

size_t Entity::ReproductionCost() const
{
	return genom.ReplicateCost();
}

const Genome& Entity::GetGenome() const
{
	return genom;
}
Genome& Entity::GetGenome()
{
	return genom;
}

void Entity::Tic()
{
	age++;
}

void Entity::SetGenome(Genome value)
{
	genom = value;
}
void Entity::Age(unsigned short value)
{
	age = value;
}
void Entity::MaxAge(unsigned short value)
{
	max_age = value;
}
void Entity::Energy(unsigned short value)
{
	energy = value;
}
