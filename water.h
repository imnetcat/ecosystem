#pragma once
#include "structure.h"

class Water : public Structure
{
public:
	Water(unsigned short lv) 
		: Structure({ 105, 170, 255 }, false, true, false, lv, 0.9) {}
};
