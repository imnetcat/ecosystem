#include "Entity.h"
#include <array>

Entity::Entity()
	:
	position(0, 0),
	view(view_side::top),
	age(0),
	max_age(0),
	hp(0),
	energy(0),
	defence(0),
	attack(0),
	max_hp(0),
	max_energy(0)
{}

Entity::Entity(
	size_t x,
	size_t y,
	view_side view,
	unsigned short max_hp,
	unsigned short energy,
	unsigned short max_energy,
	unsigned short max_age,
	double defence,
	double attack,
	Genome g)
	:
	position(x, y),
	view(view),
	age(0),
	max_age(max_age),
	max_hp(max_hp),
	hp(max_hp),
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

	unsigned short dif = attack * hp;
	if (hp < dif)
	{
		hp = 0;
		return false;
	}
	else
	{
		hp -= dif;
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

view_side Entity::GetView() const
{
	return view;
}
void Entity::SetView(view_side val)
{
	view = val;
}

void Entity::IncreaceEnergy(unsigned short value)
{
	unsigned short dif = max_hp - hp;
	if (dif != 0)
	{
		if (value > dif)
		{
			IncreaceHp(dif);
			value -= dif;
		}
		else
		{
			IncreaceHp(dif - value);
			value = 0;
		}
	}

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

	DecreaceHp(value);
}

void Entity::IncreaceHp(unsigned short value)
{
	if (hp + value > max_hp)
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

unsigned short Entity::Energy() const
{
	return energy;
}
unsigned short Entity::MaxEnergy() const
{
	return max_energy;
}

unsigned short Entity::Hp() const
{
	return hp;
}

bool Entity::IsDead() const
{
	return !hp || age > max_age;
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
void Entity::View(view_side value)
{
	view = value;
}
void Entity::Energy(unsigned short value)
{
	energy = value;
}
void Entity::Hp(unsigned short value)
{
	hp = value;
}