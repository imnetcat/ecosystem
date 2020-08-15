#pragma once
#include "object.h"
#include "ration.h"
#include "item.h"
#include <memory>

class Entity : public Object
{
protected:
	unsigned short age;
	unsigned short hp;
	unsigned short energy;
	unsigned int accumulated_energy;
public:
	Entity(unsigned short h, unsigned short e);

	virtual void Eat(unsigned short) = 0;
	virtual std::shared_ptr<Entity> Furcation() = 0;
	virtual RGBColor Color() = 0;
	virtual ration Ration() = 0;

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
