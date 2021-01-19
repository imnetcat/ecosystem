#include "cell.h"

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
	float defence,
	unsigned short attack,
	Genome g,
	const std::vector<Trigger> organelles)
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
	reproduction_cost(repr_cost),
	organelles(organelles)
{
	auto size = static_cast<unsigned int>(Protein::Count);
	proteins.reserve(size);
	for (unsigned int i = 0; i < size; i++)
	{
		proteins[static_cast<Protein>(i % size)] = 0;
	}
}

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
	proteins = cell.proteins;
	organelles = cell.organelles;
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
	proteins = cell.proteins;
	organelles = cell.organelles;
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
	proteins = cell.proteins;
	organelles = cell.organelles;
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
	proteins = cell.proteins;
	organelles = cell.organelles;
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

bool Cell::Outline(view_settings)
{
	return true;
}
unsigned short Cell::Age()
{
	return age;
}
unsigned short Cell::MaxAge()
{
	return max_age;
}
bool Cell::Defenced(unsigned short attack_power)
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
unsigned short Cell::Attack()
{
	return attack;
}
void Cell::AttackUp()
{
	attack++;
}
float Cell::Defence()
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
	return !hp || age == max_age;
}

size_t Cell::ReprodutionCost()
{
	return reproduction_cost;
}

const Genome& Cell::GetGenome() const
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
		if (lhs_genom[index] != rhs_genom[index])
			count_of_non_equal++;

		if (count_of_non_equal > BORDER)
			return false;
	}

	return true;
}

std::unordered_map<Protein, unsigned long>& Cell::Proteins()
{
	return proteins;
}
const std::unordered_map<Protein, unsigned long>& Cell::Proteins() const
{
	return proteins;
}

const std::vector<Trigger>& Cell::Organelles()
{
	return organelles;
}

RGBColor Cell::TerrainColor()
{
	return { 13, 168, 19 };
}
RGBColor Cell::MineralsColor()
{
	return { 209, 209, 209 };
}
RGBColor Cell::EnergyColor()
{
	if (energy < (MAX_ENERGY / 2))
	{
		return { 255, static_cast<unsigned char>(255 * (energy / (float)(MAX_ENERGY / 2))), 0 };
	}
	else
	{
		return { static_cast<unsigned char>(255 - 255 * (energy / (float)MAX_ENERGY)), 255, 0 };
	}
}
RGBColor Cell::SpeciesColor()
{
	return Species();
}
RGBColor Cell::AgeColor()
{
	unsigned char c = static_cast<unsigned char>(255 - 255 * ((float)age / max_age));
	return { c, c, c };
}
RGBColor Cell::HpColor()
{
	if (hp < (MAX_HP / 2))
	{
		return { 191, static_cast<unsigned char>(191 * (hp / (float)(MAX_HP / 2))), 0 };
	}
	else
	{
		return { static_cast<unsigned char>(191 * ((float)(MAX_HP / 2) / hp)), 191, 0 };
	}
}
RGBColor Cell::SurvivalColor()
{
	switch (CellSuccessRule(energy, age, max_age))
	{
	case Cell::CellSuccess::fail:
		return { 255, 21, 0 };
		break;
	case Cell::CellSuccess::normal:
		return { 255, 225, 0 };
		break;
	case Cell::CellSuccess::good:
		return { 0, 194, 0 };
		break;
	default:
		return { 255, 225, 0 };
		break;
	}
}

void Cell::Tic()
{
	auto p = genom.Read();
	if (!proteins.count(p))
	{
		proteins[p] = 1;
	}
	else
	{
		proteins[p]++;
	}

	age++;
}

RGBColor Cell::RationColor()
{
	return { 0, 143, 31 }; // green
}

RGBColor Cell::Species()
{
	return genom.Hash();
}
void Cell::Reproduction(Cell& new_cell)
{
	CellSuccess isSuccess = CellSuccessRule(energy, age, max_age);
	float newMutationChanceK = 0;
	switch (isSuccess)
	{
	case Cell::CellSuccess::fail:
		newMutationChanceK = 0.01;
		break;
	case Cell::CellSuccess::good:
		newMutationChanceK = -0.01;
		break;
	}
	Genome new_genom = genom.Replicate(newMutationChanceK);

	short max_age_koef = 0;
	switch (isSuccess)
	{
	case Cell::CellSuccess::fail:
		max_age_koef = -1;
		break;
	case Cell::CellSuccess::good:
		max_age_koef = 1;
		break;
	}

	unsigned short new_max_age = max_age + max_age_koef;
	if (new_max_age > 1000) new_max_age = 1000;
	if (new_max_age < 2) new_max_age = 2;

	std::vector<Trigger> new_organelles = organelles;

	new_cell.SetPosition(x, y);
	new_cell.Energy(100);
	new_cell.MaxAge(new_max_age);
	new_cell.ReproductionCost(reproduction_cost);
	new_cell.Defence(defence);
	new_cell.Attack(attack);
	new_cell.SetGenome(new_genom);
	new_cell.Organelles(new_organelles);
}

Cell::CellSuccess Cell::CellSuccessRule(size_t accumulated_energy, unsigned short age, unsigned short max_age)
{
	unsigned int success_border = MAX_ENERGY * (float(age) / (max_age / 2));
	return (accumulated_energy > success_border) ? CellSuccess::good :
		(accumulated_energy > (success_border / 2) ? CellSuccess::normal : CellSuccess::fail);
}

void Cell::SetGenome(Genome value)
{
	genom = value;
}
void Cell::Organelles(std::vector<Trigger> value)
{
	organelles = value;
}
void Cell::ReproductionCost(size_t value)
{
	reproduction_cost = value;
}
void Cell::Defence(float value)
{
	defence = value;
}
void Cell::Attack(unsigned short value)
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