#include "Entity.h"
using namespace Ecosystem::Logic;

Entity::Entity()
	:
	position(0, 0),
	age(0),
	max_age(0),
	energy(0),
	defence(0),
	attack(0),
	max_energy(0)
{}

Entity::Entity(
	size_t x,
	size_t y,
	unsigned short energy,
	unsigned short max_energy,
	unsigned short max_age,
	double defence,
	double attack,
	Genome g)
	:
	position(x, y),
	age(0),
	max_age(max_age),
	energy(energy),
	max_energy(max_energy),
	defence(defence),
	attack(attack),
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
bool Entity::Defencing(double attack)
{
	if (attack < defence)
	{
		return false;
	}
	
	attack -= defence;

	unsigned short dif = attack * energy;
	if (energy < dif)
	{
		energy = 0;
		return false;
	}
	else
	{
		energy -= dif;
		defence -= 0.01; // defence skill up
		return true;
	}
}
double Entity::Attack() const
{
	return attack;
}
void Entity::AttackUp()
{
	attack += 0.01;
}
double Entity::Defence() const
{
	return defence;
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
void Entity::Defence(double value)
{
	defence = value;
}
void Entity::Attack(double value)
{
	attack = value;
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
