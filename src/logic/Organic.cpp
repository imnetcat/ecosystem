#include "Organic.h"

Organic::Organic() : energy(0) {}
Organic::Organic(size_t x, size_t y, unsigned __int64 energy)
	: Object(x, y),
	energy(energy)
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