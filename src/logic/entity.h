#pragma once
#include "genome.h"
#include "position.h"
#include "../utils/pool.h"
namespace Ecosystem
{
	namespace Logic
	{
		class Entity : public position
		{
		protected:
			unsigned short age;
			unsigned short max_age;
			unsigned short hp;
			unsigned short max_hp;
			unsigned short energy;
			unsigned short max_energy;
			double defence;
			double attack;
			Genome genom;
			unsigned char carnivorousing = 0;
			unsigned char fotosintesis = 0;
			unsigned char mineraling = 0;
		public:
			Entity();
			Entity(
				size_t x,
				size_t y,
				unsigned short max_hp,
				unsigned short energy,
				unsigned short max_energy,
				unsigned short max_age,
				double defence,
				double attack,
				Genome g
			);


			bool IsDead() const;

			size_t ReproductionCost() const;

			const Genome& GetGenome() const;
			Genome& GetGenome();

			void Tic();

			bool Defencing(double);
			double Defence() const;
			double Attack() const;
			void AttackUp();
			unsigned short Age() const;
			unsigned short MaxAge() const;
			void IncreaceEnergy(unsigned short value);
			void DecreaceEnergy(unsigned short value);
			void IncreaceHp(unsigned short value);
			void DecreaceHp(unsigned short value);
			unsigned short Energy() const;
			unsigned short MaxEnergy() const;
			unsigned short Hp() const;

			void SetGenome(Genome value);
			void Defence(double);
			void Attack(double);
			void Age(unsigned short);
			void MaxAge(unsigned short);
			void Energy(unsigned short);
			void Hp(unsigned short);
		};

		using EntitiesIterator = pool<Entity>::iterator;
	}
}