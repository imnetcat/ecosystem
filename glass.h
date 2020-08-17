#pragma once
#include "structure.h"

class Glass : public Structure
{
public:
	Glass() 
		: Structure({ 245, 245, 245 }, false, 0.95) {}

};
