#pragma once
#include "genome.h"
#include "position.h"
#include "../utils/pool.h"
namespace ecosystem
{
	namespace logic
	{
		class Entity : public position
		{
		protected:
			unsigned short age;
			unsigned short max_age;
			unsigned short energy;
			unsigned short max_energy;
			Genome genom;
			unsigned char carnivorousing = 0;
			unsigned char fotosintesis = 0;
			unsigned char mineraling = 0;
		public:
			Entity();
			Entity(
				int x,
				int y,
				unsigned short energy,
				unsigned short max_energy,
				Genome g,
				unsigned short max_age = 50
			);


			bool IsDead() const;

			size_t ReproductionCost() const;

			const Genome& GetGenome() const;
			Genome& GetGenome();

			void Tic();

			unsigned short Age() const;
			unsigned short MaxAge() const;
			void IncreaceEnergy(unsigned short value);
			void DecreaceEnergy(unsigned short value);
			unsigned short Energy() const;
			unsigned short MaxEnergy() const;

			void SetGenome(Genome value);
			void Age(unsigned short);
			void MaxAge(unsigned short);
			void Energy(unsigned short);
		};

		using EntitiesIterator = pool<Entity>::iterator;
	}
}