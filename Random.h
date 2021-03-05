#pragma once
#include <random>
#include <chrono>

class Random
{
private:
	static std::mt19937_64 engine;
	static std::uniform_int_distribution<unsigned __int64> distributor;
public:

	// Return random numbers in range [0, max)
	int Generate(int max);
	size_t Generate(size_t max);
};
