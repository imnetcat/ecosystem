#pragma once
#include "Object.h"

class Material : public Object
{
public:
	Material(RGBColor color, bool walkable, float transparency, GravityEffect gravity);
};
