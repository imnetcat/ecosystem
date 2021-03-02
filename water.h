#pragma once
#include "Material.h"

class Water : public Material
{
public:
	Water() 
		: Material({ 0, 121, 235 }, true, 0.99) {}
};
