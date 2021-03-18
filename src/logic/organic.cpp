#include "organic.h"
using namespace Ecosystem::Logic;

Organic::Organic() : energy(0) {}
Organic::Organic(size_t x, size_t y, unsigned __int64 energy)
	: position(x, y)
	, energy(energy)
{}

unsigned __int64 Organic::Energy()
{
	return energy;
}

void Organic::Increace(unsigned __int64 value)
{
	energy += value;
}

void Organic::Decreace(unsigned __int64 value)
{
	if (value > energy)
	{
		energy = 0;
	}
	else
	{
		energy -= value;
	}
}
