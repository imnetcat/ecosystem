#include "cell.h"
#include "config.h"

Cell::Cell()
	:
	x(0),
	y(0),
	view(static_cast<view_side>(rand() % 8)),
	age(0),
	max_age(0),
	hp(0),
	energy(0),
	defence(0),
	attack(0),
	reproduction_cost(0)
{}

Cell::Cell(
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

Cell::Cell(Cell&& cell)
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
Cell::Cell(const Cell& cell)
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
Cell& Cell::operator = (const Cell& cell)
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
Cell& Cell::operator = (Cell&& cell)
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

size_t Cell::GetX() const
{
	return x;
}
size_t Cell::GetY() const
{
	return y;
}
void Cell::SetPosition(size_t nx, size_t ny)
{
	x = nx;
	y = ny;
}

unsigned short Cell::Age()
{
	return age;
}
unsigned short Cell::MaxAge()
{
	return max_age;
}
bool Cell::Defencing(double attack)
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
double Cell::Attack()
{
	return attack;
}
void Cell::AttackUp()
{
	attack += 0.01;
}
double Cell::Defence()
{
	return defence;
}

view_side Cell::GetView()
{
	return view;
}
void Cell::SetView(view_side val)
{
	view = val;
}

void Cell::IncreaceEnergy(unsigned short value)
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

void Cell::DecreaceEnergy(unsigned short value)
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

void Cell::IncreaceHp(unsigned short value)
{
	if (hp + value > MAX_HP)
		hp = 100;
	else
		hp += value;
}

void Cell::DecreaceHp(unsigned short value)
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

unsigned short Cell::Energy()
{
	return energy;
}

unsigned short Cell::Hp()
{
	return hp;
}

bool Cell::IsDead()
{
	return !hp || age > max_age;
}

size_t Cell::ReprodutionCost()
{
	return reproduction_cost;
}

const Genome& Cell::GetGenome() const
{
	return genom;
}
Genome& Cell::GetGenome()
{
	return genom;
}

bool Cell::IsFriendly(const Cell& cell)
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

void Cell::Tic()
{
	age++;
}

RGBColor Cell::Species()
{
	return genom.Hash();
}
size_t Cell::Reproduction(Cell& new_cell)
{
	Success isSuccess = SuccessRule();
	double newMutationChanceK = 0;
	switch (isSuccess)
	{
	case Cell::Success::fail:
		newMutationChanceK = 0.01;
		break;
	case Cell::Success::good:
		newMutationChanceK = -0.01;
		break;
	}
	Genome new_genom = genom.Replicate(newMutationChanceK);

	short max_age_koef = 0;
	switch (isSuccess)
	{
	case Cell::Success::fail:
		max_age_koef = -1;
		break;
	case Cell::Success::good:
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

Cell::Success Cell::SuccessRule()
{
	unsigned int success_border = MAX_ENERGY * (double(age) / (max_age));
	success_border /= 2;
	return (energy > success_border) ? Success::good :
		(energy > (success_border / 2) ? Success::normal : Success::fail);
}

void Cell::SetGenome(Genome value)
{
	genom = value;
}
void Cell::ReproductionCost(size_t value)
{
	reproduction_cost = value;
}
void Cell::Defence(double value)
{
	defence = value;
}
void Cell::Attack(double value)
{
	attack = value;
}
void Cell::Age(unsigned short value)
{
	age = value;
}
void Cell::MaxAge(unsigned short value)
{
	max_age = value;
}
void Cell::View(view_side value)
{
	view = value;
}
void Cell::Energy(unsigned short value)
{
	energy = value;
}
void Cell::Hp(unsigned short value)
{
	hp = value;
}