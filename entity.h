#pragma once
#include "object.h"
#include "item.h"
#include "gen.h"
#include <memory>

class Structure;

enum view_side
{
	top,
	right_top,
	right,
	right_bottom,
	bottom,
	left_bottom,
	left,
	left_top,
};

class Entity : public Object
{
protected:
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short energy;
	double defence;
	unsigned short attack;
public:
	Entity(unsigned short h, unsigned short e, unsigned short max_age, double def, unsigned short attck);
	virtual Genome GetGenome() = 0;
	virtual size_t SeparationCost() = 0;
	virtual size_t BirthCost() = 0;
	virtual void Tic(std::vector<Gen::Command>& commands) = 0;
	virtual void Eat(Structure*, Structure*) = 0;
	virtual RGBColor Species() = 0;

	bool Outline(view_settings) override;
	bool Defenced(unsigned short);
	double Defence();
	unsigned short Attack();
	void AttackUp();
	unsigned short Age();
	unsigned short MaxAge();
	view_side Entity::GetView();
	void Entity::SetView(view_side val);
	void IncreaceEnergy(unsigned short value);
	void DecreaceEnergy(unsigned short value);
	void IncreaceHp(unsigned short value);
	void DecreaceHp(unsigned short value);
	unsigned short Energy();
	unsigned short Hp();
};
