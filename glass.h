#pragma once
#include "structure.h"

class Glass : public Structure
{
public:
	Glass(unsigned short lv) 
		: Structure({ 245, 245, 245 }, false, false, false, lv, 1.0) {}

};
