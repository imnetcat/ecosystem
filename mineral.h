#pragma once
#include "object.h"

struct Mineral : Object
{
	Mineral(unsigned short e) : energy(e) {}

	ObjectColor Color() override
	{
		return { 0, 227, 208 };
	}


	void IncreaceEnergy(unsigned short value) override
	{
		energy += value;
	}

	Command Tic() override
	{
		return move_bottom;
	}

	unsigned short energy = 100;
};