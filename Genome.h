#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include <map>
#include <vector>
#include <limits>
#include "Gen.h"
#include "Random.h"
#include <SFML/Graphics/Color.hpp>

using RGBColor = sf::Color;

enum class Ration
{
	omnivorous,
	entities,
	light,
	organic,
	entites_organic,
	entites_light,
	light_organic
};

enum class Coefficient
{
	enlarge,
	reduce,
	unchanged
};

#include "ListPool.h"

class Genome
{
private:
	unsigned __int64 genom;		// 8 bytes
	unsigned __int32 props;		// 4 bytes
	unsigned __int8 args;		// 1 byte
	unsigned __int8 cursor;		// 1 byte

	unsigned __int64 generation;
	unsigned __int8 mutationChance;
	Ration ration;
	RGBColor species;
	Random random;
	unsigned __int16 replicate_cost;

	constexpr static const unsigned __int8 genom_size = sizeof(genom) * 8;
	constexpr static const unsigned __int8 args_size = sizeof(args) * 8;
	constexpr static const unsigned __int64 genome_max = std::numeric_limits<unsigned __int64>().max();
	constexpr static const unsigned __int64 args_max = std::numeric_limits<unsigned __int8>().max();
	constexpr static const unsigned __int8 trigger_max = static_cast<unsigned __int8>(Trigger::Count);

	static const std::uniform_int_distribution<unsigned __int64> distributor;
	static unsigned __int64 start_data[64];
	static std::mt19937_64 engine;
	static unsigned __int64 world_seed;

	void Construct();
public:
	Genome();
	// mutationChance must be from 0 to 100
	Genome(
		unsigned __int64 genom,
		unsigned __int8 args,
		unsigned __int64 generation,
		unsigned __int8 mutationChance
	);

	Gen Read();
	unsigned __int64 Data() const;
	unsigned __int64 Generation() const;
	unsigned __int8 MutationChance() const;
	inline unsigned __int8 Size() const;
	const RGBColor& Species() const;
	Ration Ration() const;
	unsigned __int16 ReplicateCost() const;

	static void Init(unsigned __int64 world_seed);
	Genome Replicate(Coefficient coef);
};
