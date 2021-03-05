#include "Random.h"

std::mt19937 Random::engine = std::mt19937(std::random_device()());

bool Random::Chance(float chance)
{
	std::uniform_real_distribution<float> dist(0, 1);
	return dist(engine) < chance;
}

unsigned __int8 Random::Generate(unsigned __int8 max)
{
	if (!max) return 0;
	std::uniform_int_distribution<unsigned __int16> dist(0, max - 1);
	return dist(engine);
}
int Random::Generate(int max)
{
	if (!max) return 0;
	std::uniform_int_distribution<int> dist(0, max - 1);
	return dist(engine);
}
size_t Random::Generate(size_t max)
{
	if (!max) return 0;
	std::uniform_int_distribution<size_t> dist(0, max - 1);
	return dist(engine);
}