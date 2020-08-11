#pragma once
#include "gen.h"

// абстрактное живое существо
class Entity
{
public:
	explicit Entity(Gen g) : genom(g)
	{

	}
	Gen genom;
};
