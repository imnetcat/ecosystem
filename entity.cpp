#include "entity.h"

Entity::Entity(unsigned short h, unsigned short e, unsigned short ae) :
		age(0),
		hp(h),
		energy(e),
		accumulated_energy(ae) {}

void Entity::IncreaceAccEnergy(unsigned short value)
{
	if (accumulated_energy < 32000)
		accumulated_energy += value;
}
void Entity::DecreaceAccEnergy(unsigned short value)
{
	if (accumulated_energy > value)
		accumulated_energy -= value;
}

unsigned short Entity::AccEnergy()
{
	return accumulated_energy;
}

void Entity::IncreaceEnergy(unsigned short value)
{
	if (hp < 100)
	{
		IncreaceHp(value);
	}
	else
	{
		if (energy + value > 100)
		{
			accumulated_energy += value + energy - 100;
			energy = 100;
		}
		else
			energy += value;
	}
}

void Entity::DecreaceEnergy(unsigned short value)
{
	if (energy > value)
	{
		energy -= value;
	}
	else if (energy > 0)
	{
		DecreaceHp(value - energy);
		energy = 0;
	}
	else
	{
		DecreaceHp(value);
	}
}

void Entity::IncreaceHp(unsigned short value)
{
	if (hp + value > 100)
		hp = 100;
	else
		hp += value;
}

void Entity::DecreaceHp(unsigned short value)
{
	if (hp > value)
	{
		hp -= value;
	}
	else
	{
		hp = 0;
	}
}

unsigned short Entity::Energy()
{
	return energy;
}

unsigned short Entity::Hp()
{
	return hp;
}
