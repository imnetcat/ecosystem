#pragma once
#include "structure.h"

class Water : public Structure
{
public:
	Water() 
		: Structure({ 105, 170, 255 }, true, 0.92) {}
};
