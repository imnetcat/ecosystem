#pragma once
#include <random>

class random
{
private:
	static std::mt19937_64 engine;
	static std::uniform_int_distribution<unsigned __int64> distributor;
public:

	// Return random numbers in range [0, max)
	static size_t Generate(size_t max);
	// Return random numbers in range [0, distributor maximum)
	static size_t Generate();
};
