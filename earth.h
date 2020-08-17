#pragma once
#include "structure.h"

class Earth : public Structure
{
public:
	Earth()
		: Structure({ 150, 84, 12 }, false, 0) {}
};
