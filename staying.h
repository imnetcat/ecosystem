#pragma once
#include "organelle.h"

class Staying : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
	inline unsigned int MaintenanceCost() const override
	{
		return 1;
	}
	inline unsigned int CreationCost() const override
	{
		return 2;
	}

};