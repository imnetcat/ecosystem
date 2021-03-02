#pragma once
#include "Object.h"

class Organic : public Object
{
public:
	Organic() : Object({ 136, 113, 137 }, true, 0.5, GravityEffect::Down) { }
	
	void Rot()
	{

	}
};
