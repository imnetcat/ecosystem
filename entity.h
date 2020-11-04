#pragma once
#include "object.h"
#include "genome.h"
#include <memory>

class Entity : public Object
{
protected:
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short energy;
	float defence;
	unsigned short attack;
public:
	Entity(unsigned short h, unsigned short e, unsigned short max_age, float def, unsigned short attck);
	virtual Genome GetGenome() = 0;
	virtual size_t SeparationCost() = 0;
	virtual size_t BirthCost() = 0;
	virtual RGBColor Species() = 0;
	virtual bool IsDead() = 0;

	bool Outline(view_settings) override;
	bool Defenced(unsigned short);
	float Defence();
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
