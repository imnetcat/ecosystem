#pragma once
#include "Object.h"
#include "config.h"
#include "ListPool.h"

class Organic : public Object
{
private:
	unsigned __int64 energy;
public:
	Organic();
	Organic(size_t x, size_t y, unsigned __int64 energy);

	unsigned __int64 Energy();
	void Increace(unsigned __int64 value);
	void Decreace(unsigned __int64 value);
};

using OrganicIterator = ListPool<Organic, ENVIRONMENT_SIZE_Y* ENVIRONMENT_SIZE_X>::iterator;
