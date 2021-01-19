#pragma once
#include "protein.h"
#include "map_terrain.h"
#include <unordered_map>
class Organelle
{
public:
	virtual void Event(size_t& x, size_t& y) const = 0;
	virtual inline unsigned int MaintenanceCost() const = 0;
	virtual inline unsigned int CreationCost() const = 0;
};
