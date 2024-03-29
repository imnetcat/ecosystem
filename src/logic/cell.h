#pragma once
#include "Organic.h"
#include "Entity.h"
namespace ecosystem
{
	namespace logic
	{
		class cell : private position
		{
		protected:
			OrganicIterator object;
			EntitiesIterator entity;
			bool has_entity = false;
			bool has_object = false;
			unsigned int light_power;
			double light_coef;
		public:

			int x() const;
			int y() const;

			void Init(unsigned int light_power, size_t x, size_t y);
			void SetEntity(EntitiesIterator);
			void DelEntity();
			EntitiesIterator GetEntity() const;
			bool ContainsEntity() const;

			unsigned int LightPower() const;

			void SetOrganic(OrganicIterator organic);
			void AddOrganic(size_t value);
			void DelOrganic();
			OrganicIterator GetOrganic() const;
			bool ContainsOrganic() const;
		};
	}
}
