#pragma once
#include "organelle.h"

class Mineraling : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
	inline unsigned int Mineraling::MaintenanceCost() const override
	{
		return 10;
	}
	inline unsigned int Mineraling::CreationCost() const override
	{
		return 20;
	}
};