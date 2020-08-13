#pragma once
#include "structure.h"

struct Mineral : Structure
{
	Mineral(unsigned short e) 
		: Structure({ 0, 227, 208 }, true, true, true, 0, 1.0), energy(e)  {}

	unsigned short energy;
};