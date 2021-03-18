#pragma once
#include "cell.h"
namespace Ecosystem
{
	namespace Logic
	{
		class World
		{
		public:
			explicit World(
				unsigned int width,
				unsigned int height,
				unsigned short light_power,
				unsigned short max_orginic_to_eat,
				unsigned short max_entities_to_eat,
				unsigned short max_energy,
				unsigned short max_age
			);
			~World();

			void Reload();

			void Update();

			Coefficient SuccessRule(pool<Entity>::const_iterator) const;

			// Getters
			size_t MaxGeneration() const;
			cell** const Terrain() const;
			const pool<Entity>& Entities() const;
			const pool<Organic>& Organics() const;
			unsigned int Width() const;
			unsigned int Height() const;
		protected:

			const unsigned int width;
			const unsigned int height;

			const unsigned short light_power;
			const unsigned short max_age;
			const unsigned short max_energy;
			const unsigned short max_organic_to_eat;
			const unsigned short max_entities_to_eat;

			size_t max_generation;
			cell** terrain;
			pool<Entity> entities;
			pool<Organic> organic;

		private:

			EntitiesIterator EntityDie(EntitiesIterator);

			bool GetViewPos(unsigned __int8 arg, position& pos);

			EntitiesIterator Reproduction(EntitiesIterator parent_entity, const position& pos);
			void Separationing(unsigned __int8 args, EntitiesIterator);
			void Birthing(unsigned __int8 args, EntitiesIterator);
			void Attacking(unsigned __int8 args, EntitiesIterator);
			void EatOrganic(EntitiesIterator);
			void Moving(unsigned __int8 args, EntitiesIterator);
			void Photosynthesing(EntitiesIterator);
		};
	}
}