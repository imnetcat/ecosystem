#include "entity.h"

Entity::Entity(unsigned short h, unsigned short e, unsigned short ma, double def, unsigned short attck) :
		view(top),
		age(0),
		max_age(ma),
		hp(h),
		energy(e),
		defence(def),
		attack(attck) {}


bool Entity::Outline(view_settings)
{
	return true;
}
unsigned short Entity::Age()
{
	return age;
}
unsigned short Entity::MaxAge()
{
	return max_age;
}
bool Entity::Defenced(unsigned short attack_power)
{
	unsigned short dif = attack_power * defence;
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
unsigned short Entity::Attack()
{
	return attack;
}
void Entity::AttackUp()
{
	attack++;
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
