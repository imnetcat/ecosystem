#pragma once
#include "gen.h"
#include "../utils/pool.h"
#include "../utils/random.h"
#define _CRT_SECURE_NO_WARNINGS // For wxWidgets lib
#include <wx/colour.h>
#include "coefficient.h"
#include <limits>
namespace ecosystem
{
	namespace logic
	{
		using Color = wxColor;

		class Genome
		{
		private:
			unsigned __int64 genome;		// 8 bytes
			unsigned __int32 props;		// 4 bytes
			unsigned __int8 args;		// 1 byte
			unsigned __int8 cursor;		// 1 byte

			unsigned __int64 generation;
			unsigned __int8 mutationChance;
			Color ration;
			Color species;
			random random;
			unsigned __int16 replicate_cost;

			unsigned __int8 genome_size = sizeof(genome) * 8;
			constexpr static const unsigned __int8 args_size = sizeof(args) * 8;
			constexpr static const unsigned __int64 genome_max = std::numeric_limits<unsigned __int64>().max();
			constexpr static const unsigned __int64 args_max = std::numeric_limits<unsigned __int8>().max();
			constexpr static const unsigned __int8 trigger_max = static_cast<unsigned __int8>(Operation::Count);

			static const std::uniform_int_distribution<unsigned __int64> distributor;
			static unsigned __int64 start_data[64];
			static std::mt19937_64 engine;
			static unsigned __int64 genome_seed;

			void Construct();
		public:
			Genome();
			// mutationChance must be from 0 to 100
			Genome(
				unsigned __int64 genome,
				unsigned __int8 genome_size,
				unsigned __int8 args,
				unsigned __int64 generation,
				unsigned __int8 mutationChance
			);

			Gen Read();
			unsigned __int8 Args() const;
			unsigned __int64 Data() const;
			unsigned __int64 Generation() const;
			unsigned __int8 MutationChance() const;
			inline unsigned __int8 Size() const;
			const Color& Species() const;
			const Color& Ration() const;
			unsigned __int16 ReplicateCost() const;

			static void Init(unsigned __int64 genome_seed);

			Genome Replicate(Coefficient coef);
		};
	}
}
