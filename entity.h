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

	void Symbiosis()
	{
		meat += meat < 225 ? 30 : (255 - meat);
		light += light < 225 ? 30 : (255 - light);
		minerals += minerals < 225 ? 30 : (255 - minerals);
	}
	void IncreaceMeat()
	{
		meat += meat < 180 ? 75 : (255 - meat);
		light -= light > 15 ? 15 : light;
		minerals -= minerals > 15 ? 15 : minerals;
	}
	void IncreaceLight()
	{
		meat -= meat > 30 ? 30 : meat;
		light += light < 240 ? 15 : (255 - light);
		minerals -= minerals > 15 ? 15 : minerals;
	}
	void IncreaceMinerals()
	{
		meat -= meat > 30 ? 30 : meat;
		light -= light > 15 ? 15 : light;
		minerals += minerals < 240 ? 15 : (255 - minerals);
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
	virtual bool IsFriendly(Gen) = 0;

	unsigned short Age();
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
