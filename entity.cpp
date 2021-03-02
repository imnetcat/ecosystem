#include "entity.h"
#include "config.h"

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
	attack(0),
	reproduction_cost(0)
{}

Entity::Entity(
	size_t x,
	size_t y,
	unsigned short energy,
	unsigned short max_age,
	size_t repr_cost,
	double defence,
	double attack,
	Genome g)
	:
	x(x),
	y(y),
	view(static_cast<view_side>(rand() % 8)),
	age(0),
	max_age(max_age),
	hp(MAX_HP),
	energy(energy),
	defence(defence),
	attack(attack),
	genom(g),
	reproduction_cost(repr_cost)
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
	reproduction_cost = cell.reproduction_cost;
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
	reproduction_cost = cell.reproduction_cost;
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
	reproduction_cost = cell.reproduction_cost;
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
	reproduction_cost = cell.reproduction_cost;
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

size_t Entity::ReprodutionCost()
{
	return reproduction_cost;
}

const Genome& Entity::GetGenome() const
{
	return genom;
}
Genome& Entity::GetGenome()
{
	return genom;
}

bool Entity::IsFriendly(const Entity& cell)
{
	const unsigned short BORDER = 8;
	size_t count_of_non_equal = 0;
	auto lhs_genom = GetGenome().data;
	auto rhs_genom = cell.GetGenome().data;

	if (lhs_genom.size() != rhs_genom.size())
		return false;

	for (size_t index = 0; index < lhs_genom.size(); index++)
	{
		if (lhs_genom[index].trigger != rhs_genom[index].trigger && 
			lhs_genom[index].args != rhs_genom[index].args)
			count_of_non_equal++;

		if (count_of_non_equal > BORDER)
			return false;
	}

	return true;
}

void Entity::Tic()
{
	age++;
}

RGBColor Entity::Species()
{
	return genom.Hash();
}
size_t Entity::Reproduction(Entity& new_cell)
{
	Success isSuccess = SuccessRule();
	double newMutationChanceK = 0;
	switch (isSuccess)
	{
	case Entity::Success::fail:
		newMutationChanceK = 0.01;
		break;
	case Entity::Success::good:
		newMutationChanceK = -0.01;
		break;
	}
	Genome new_genom = genom.Replicate(newMutationChanceK);

	short max_age_koef = 0;
	switch (isSuccess)
	{
	case Entity::Success::fail:
		max_age_koef = -1;
		break;
	case Entity::Success::good:
		max_age_koef = 1;
		break;
	}

	unsigned short new_max_age = max_age + max_age_koef;
	if (new_max_age > 1000) new_max_age = 1000;
	if (new_max_age < 2) new_max_age = 2;

	new_cell.SetPosition(x, y);
	new_cell.Energy(100);
	new_cell.Age(0);
	new_cell.MaxAge(new_max_age);
	new_cell.ReproductionCost(reproduction_cost);
	new_cell.Defence(defence);
	new_cell.Attack(attack);
	new_cell.SetGenome(new_genom);
	new_cell.Hp(MAX_HP);

	return new_genom.generation;
}

Entity::Success Entity::SuccessRule()
{
	unsigned int success_border = MAX_ENERGY * (double(age) / (max_age));
	return (energy > success_border) ? Success::good :
		(energy > (success_border / 2) ? Success::normal : Success::fail);
}

void Entity::SetGenome(Genome value)
{
	genom = value;
}
void Entity::ReproductionCost(size_t value)
{
	reproduction_cost = value;
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