#pragma once
#include "object.h"
#include "item.h"
#include "gen.h"
#include <memory>

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

enum ration_type
{
	fotosintesis,// фотосинтез
	eat_minerals,	 // поедание минералов на своей позиции
	attack,		 // атака клетки если её геном отличается более чем на 2 комманды
};

class Ration
{
private:
	ration_type _type;
public:
	Ration(ration_type type) :
		_type(type) {}

	ration_type Type()
	{
		return _type;
	}

	RGBColor Color()
	{
		switch (_type)
		{
		case minerals:
			return { 0, 14, 140 }; // blue
		case fotosintesis:
			return { 0, 143, 31 }; // green
		case attack:
			return { 199, 0, 0 }; // red
		default:
			return { 255, 255, 255 }; // white
		}
	}
	static const unsigned short length = 3;
};

class Entity : public Object
{
protected:
	Ration ration_;
	view_side view;
	unsigned short age;
	unsigned short max_age;
	unsigned short hp;
	unsigned short energy;
public:
	Entity(unsigned short h, unsigned short e, unsigned short max_age, Ration r);
	virtual Genome GetGenome() = 0;
	virtual size_t SeparationCost() = 0;
	virtual size_t BirthCost() = 0;
	virtual void Tic(std::vector<Gen::Command>& commands) = 0;

	virtual RGBColor Species() = 0;

	unsigned short Age();
	unsigned short MaxAge();
	Ration& GetRation();
	const Ration& GetRation() const;
	view_side Entity::GetView();
	void Entity::SetView(view_side val);
	void IncreaceEnergy(unsigned short value);
	void DecreaceEnergy(unsigned short value);
	void IncreaceHp(unsigned short value);
	void DecreaceHp(unsigned short value);
	unsigned short Energy();
	unsigned short Hp();
};
