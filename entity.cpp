#include "Entity.h"
#include "config.h"
#include <array>

Entity::Entity()
	:
	x(0),
	y(0),
	view(view_side::top),
	age(0),
	max_age(0),
	hp(0),
	energy(0),
	defence(0),
	attack(0)
{}

Entity::Entity(
	size_t x,
	size_t y,
	view_side view,
	unsigned short energy,
	unsigned short max_age,
	double defence,
	double attack,
	Genome g)
	:
	x(x),
	y(y),
	view(view),
	age(0),
	max_age(max_age),
	hp(MAX_HP),
	energy(energy),
	defence(defence),
	attack(attack),
	genom(g)
{}

Entity::Entity(Entity&& cell)
{
	x = cell.x;
	y = cell.y;
	view = cell.view;
	age = cell.age;
	max_age = cell.max_age;
	hp = cell.hp;
	energy = cell.energy;
	defence = cell.defence;
	attack = cell.attack;

	genom = cell.genom;
}
Entity::Entity(const Entity& cell)
{
	x = cell.x;
	y = cell.y;
	view = cell.view;
	age = cell.age;
	max_age = cell.max_age;
	hp = cell.hp;
	energy = cell.energy;
	defence = cell.defence;
	attack = cell.attack;

	genom = cell.genom;
}
Entity& Entity::operator = (const Entity& cell)
{
	x = cell.x;
	y = cell.y;
	view = cell.view;
	age = cell.age;
	max_age = cell.max_age;
	hp = cell.hp;
	energy = cell.energy;
	defence = cell.defence;
	attack = cell.attack;

	genom = cell.genom;
	return *this;
}
Entity& Entity::operator = (Entity&& cell)
{
	x = cell.x;
	y = cell.y;
	view = cell.view;
	age = cell.age;
	max_age = cell.max_age;
	hp = cell.hp;
	energy = cell.energy;
	defence = cell.defence;
	attack = cell.attack;

	genom = cell.genom;
	return *this;
}

size_t Entity::GetX() const
{
	return x;
}
size_t Entity::GetY() const
{
	return y;
}
void Entity::SetPosition(size_t nx, size_t ny)
{
	x = nx;
	y = ny;
}

unsigned short Entity::Age()
{
	return age;
}
unsigned short Entity::MaxAge()
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
double Entity::Attack()
{
	return attack;
}
void Entity::AttackUp()
{
	attack += 0.01;
}
double Entity::Defence()
{
	return defence;
}

view_side Entity::GetView()
{
	return view;
}
void Entity::SetView(view_side val)
{
	view = val;
}

void Entity::IncreaceEnergy(unsigned short value)
{
	unsigned short dif = MAX_HP - hp;
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

	if (energy + value < MAX_ENERGY)
		energy += value;
	else
		energy = MAX_ENERGY;
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
	if (hp + value > MAX_HP)
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

bool Entity::IsDead()
{
	return !hp || age > max_age;
}

size_t Entity::ReproductionCost()
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