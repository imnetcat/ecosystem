#pragma once
#include "structure.h"

class Water : public Structure
{
public:
	Water() 
		: Structure({ 176, 226, 255 }, true, 0.99) {}
};
