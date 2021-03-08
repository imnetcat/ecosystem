#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include <map>
#include <vector>
#include <limits>
#include "Gen.h"
#include "Random.h"

struct RGBColor
{
	unsigned __int8 r = 0;
	unsigned __int8 g = 0;
	unsigned __int8 b = 0;
};

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

class Genome // 16 bytes
{
private:

	unsigned __int64 genom;		// 8 bytes
	unsigned __int32 props;		// 4 bytes
	unsigned __int16 reserved;	// 2 bytes
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
	constexpr static const unsigned __int8 trigger_max = static_cast<unsigned __int8>(Trigger::Count);
	constexpr static const unsigned __int64 genome_max = std::numeric_limits<unsigned __int64>().max();
	constexpr static const unsigned __int64 args_max = std::numeric_limits<unsigned __int8>().max();
	static unsigned __int64 xr;

	void Initializing();
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

	Genome Replicate(Coefficient coef);
};
