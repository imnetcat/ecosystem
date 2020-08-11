#pragma once

#include "entity.h"

class Cell : public Entity
{
private:
	unsigned short hp = 100;
	unsigned int energy = 0;
public:
	explicit Cell(Gen g) : Entity(g) {}
	void Eat();
	void Move();
	void Eat();
};
