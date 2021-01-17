#pragma once
#include "structure.h"

class Water : public Structure
{
public:
	Water() 
		: Structure({ 0, 121, 235 }, true, 0.99) {}
};
