#pragma once
#include "structure.h"

class Air : public Structure
{
public:
	Air()
		: Structure({ 255, 255, 255 }, false, 1.0) {}
};
