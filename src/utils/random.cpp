#include "random.h"

std::mt19937_64 random::engine = std::mt19937_64(std::random_device()());
std::uniform_int_distribution<unsigned __int64> random::distributor = std::uniform_int_distribution<unsigned __int64>(0, 999'999'999'999);

size_t random::Generate(size_t max)
{
	if (!max) return 0;
	return distributor(engine) % max;
}

size_t random::Generate()
{
	return distributor(engine);
}
