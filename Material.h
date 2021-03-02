#pragma once
#include "genome.h"

#include <iostream>
#include <memory>
#include <ctime>
#include <unordered_map>
#include <map>
#include <sstream>

class Material
{
protected:
	RGBColor color;
	bool walkable;
	float transparency;
public:

	Material(RGBColor color, bool walkable, float transparency);

	bool IsWalkable();
};
