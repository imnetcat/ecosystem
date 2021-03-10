#include "Random.h"

std::mt19937_64 Random::engine = std::mt19937_64(std::random_device()());
std::uniform_int_distribution<unsigned __int64> Random::distributor = std::uniform_int_distribution<unsigned __int64>(0, 400'000);

size_t Random::Generate(size_t max)
{
	if (!max) return 0;
	return distributor(engine) % max;
}
