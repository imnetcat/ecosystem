#pragma once
#include <random>

class Random 
{
private:
	std::random_device rd;
	std::mt19937 mt;
public:
	Random() : mt(rd()) {}

	// Calculate success of chance
	bool Chance(float chance)
	{
		std::uniform_real_distribution<float> dist(0, 1);
		return dist(mt) < chance;
	}

	// Return random numbers in range [0, max)
	int Generate(int max)
	{
		if (!max) return 0;
		std::uniform_int_distribution<int> dist(0, max - 1);
		return dist(mt);
	}
	size_t Generate(size_t max)
	{
		if (!max) return 0;
		std::uniform_int_distribution<size_t> dist(0, max - 1);
		return dist(mt);
	}
};