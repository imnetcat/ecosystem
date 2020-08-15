#pragma once
#include "object.h"
#include "item.h"
#include "gen.h"
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
	virtual Gen GetGen() = 0;
	virtual void Tic(std::vector<Command>& commands) = 0;
	virtual std::shared_ptr<Entity> Reproduction() = 0;
	virtual RGBColor Color() = 0;

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
