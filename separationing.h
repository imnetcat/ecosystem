#pragma once
#include "organelle.h"

class Separationing : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
	inline unsigned int Separationing::MaintenanceCost() const override
	{
		return 10;
	}
	inline unsigned int Separationing::CreationCost() const override
	{
		return 20;
	}
};