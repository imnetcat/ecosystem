#pragma once
#include "organelle.h"

class Moving : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
	inline unsigned int Moving::MaintenanceCost() const override
	{
		return 20;
	}
	inline unsigned int Moving::CreationCost() const override
	{
		return 50;
	}
};