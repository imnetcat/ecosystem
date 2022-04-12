#pragma once
#include "position.h"
#include "../utils/pool.h"
namespace ecosystem
{
	namespace logic
	{
		class Organic : public position
		{
		private:
			unsigned __int64 energy;
		public:
			Organic();
			Organic(int x, int y, unsigned __int64 energy);

			unsigned __int64 Energy();
			void Increace(unsigned __int64 value);
			void Decreace(unsigned __int64 value);
		};

		using OrganicIterator = pool<Organic>::iterator;
	}
}