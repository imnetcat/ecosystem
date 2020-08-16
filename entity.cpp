#include "entity.h"

Entity::Entity(unsigned short h, unsigned short e, unsigned short ae, ration r) :
		view(top),
		age(0),
		hp(h),
		energy(e),
		ration_(r),
		accumulated_energy(ae) {}

ration& Entity::Ration()
{
	return ration_;
}
const ration& Entity::Ration() const
{
	return ration_;
}

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

view_side Entity::GetView()
{
	return view;
}
void Entity::SetView(view_side val)
{
	view = val;
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
	else if (accumulated_energy > value)
	{
		accumulated_energy -= value;
	}
	else if (accumulated_energy > 0)
	{
		DecreaceHp(value - accumulated_energy);
		accumulated_energy = 0;
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
