#pragma once
#include <random>

class Random 
{
private:
	static std::mt19937 engine;
public:

	// Calculate success of chance
	bool Chance(float chance);

	// Return random numbers in range [0, max)
	unsigned __int8 Generate(unsigned __int8 max);
	int Generate(int max);
	size_t Generate(size_t max);
};
