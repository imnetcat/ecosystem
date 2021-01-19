#pragma once
#include "organelle.h"

class Birthing : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
	inline unsigned int Birthing::MaintenanceCost() const override
	{
		return 30;
	}
	inline unsigned int Birthing::CreationCost() const override
	{
		return 30;
	}
};