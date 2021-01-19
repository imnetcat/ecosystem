#pragma once
#include "organelle.h"

class Birthing : public Organelle
{
public:
	void Event(size_t& x, size_t& y) const override;
};