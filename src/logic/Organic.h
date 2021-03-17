#pragma once
#include "Object.h"
#include "../utils/pool.h"

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

using OrganicIterator = pool<Organic>::iterator;
