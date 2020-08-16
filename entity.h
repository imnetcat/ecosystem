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

class ration
{
private:
	unsigned char meat;
	unsigned char light;
	unsigned char minerals;
public:
	ration() :
		meat(255),
		light(255),
		minerals(255) {}

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

	void IncreaceMeat()
	{
		if (meat != 255)
			meat += 5;
		if (light > 5)
			light -= 5;
		if (minerals > 5)
			minerals -= 5;
	}
	void IncreaceLight()
	{
		if(meat > 5)
			meat -= 5;
		if(light != 255)
			light += 5;
		if(minerals > 5)
			minerals -= 5;
	}
	void IncreaceMinerals()
	{
		if (meat > 5)
			meat -= 5;
		if (light > 5)
			light -= 5;
		if (minerals != 255)
			minerals += 5;
	}
};

class Entity : public Object
{
protected:
	ration ration_;
	view_side view;
	unsigned short age;
	unsigned short hp;
	unsigned short energy;
	unsigned int accumulated_energy;
public:
	Entity(unsigned short h, unsigned short e, unsigned short ae, ration r);
	virtual Gen GetGen() = 0;
	virtual void Tic(std::vector<Command>& commands) = 0;
	virtual std::shared_ptr<Entity> Reproduction() = 0;
	virtual RGBColor Color() = 0;
	virtual bool IsFriendly(Gen) = 0;

	ration& Ration();
	const ration& Ration() const;
	view_side Entity::GetView();
	void Entity::SetView(view_side val);
	void IncreaceAccEnergy(unsigned short value);
	void DecreaceAccEnergy(unsigned short value);
	unsigned short AccEnergy();
	void IncreaceEnergy(unsigned short value);
	void DecreaceEnergy(unsigned short value);
	void IncreaceHp(unsigned short value);
	void DecreaceHp(unsigned short value);
	unsigned short Energy();
	unsigned short Hp();
};
