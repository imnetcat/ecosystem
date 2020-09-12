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

class Ration
{
private:
	unsigned char meat;
	unsigned char light;
	unsigned char minerals;
	static const unsigned short max = 12;
public:
	Ration() :
		meat(0),
		light(0),
		minerals(0) {}

	unsigned char Meat()
	{
		return meat;
	}
	unsigned char Light()
	{
		return light;
	}
	unsigned char Minerals()
	{
		return minerals;
	}

	RGBColor Color()
	{
		if (!meat && !light && !minerals)
			return { 255, 255, 255 }; // white

		if (meat > light && meat > minerals)
			return { 199, 0, 0 }; // red
		if (light > meat && light > minerals)
			return { 0, 143, 31 }; // green
		if (minerals > meat && minerals > light)
			return { 0, 14, 140 }; // blue

	}

	void IncreaceMeat()
	{
		if(meat < max)
			meat++;
		if(light)
			light--;
		if (minerals)
			minerals--;
	}
	void IncreaceLight()
	{
		if (meat)
			meat--;
		if (light < max)
			light++;
		if (minerals)
			minerals--;
	}
	void IncreaceMinerals()
	{
		if (meat)
			meat--;
		if (light)
			light--;
		if (minerals < max)
			minerals++;
	}
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
