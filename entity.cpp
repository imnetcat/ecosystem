#include "entity.h"

Entity::Entity(unsigned short h, unsigned short e, unsigned short ae, unsigned short ma, Ration r) :
		view(top),
		age(0),
		max_age(ma),
		hp(h),
		energy(e),
		ration_(r),
		accumulated_energy(ae) {}


unsigned short Entity::Age()
{
	return age;
}
unsigned short Entity::MaxAge()
{
	return max_age;
}
Ration& Entity::GetRation()
{
	return ration_;
}
const Ration& Entity::GetRation() const
{
	return ration_;
}

void Entity::IncreaceAccEnergy(unsigned short value)
{
	if (accumulated_energy + value < MAX_ACC_ENERGY)
		accumulated_energy += value;
	else
		accumulated_energy = MAX_ACC_ENERGY;
}
void Entity::DecreaceAccEnergy(unsigned short value)
{
	if (accumulated_energy > value)
		accumulated_energy -= value;
	else
		accumulated_energy = 0;
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
			IncreaceAccEnergy(value + energy - 100);
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
		return;
	}

	value -= energy;
	energy = 0;
	
	if (accumulated_energy > value)
	{
		accumulated_energy -= value;
		return;
	}

	DecreaceHp(value - accumulated_energy);
	accumulated_energy = 0;
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
