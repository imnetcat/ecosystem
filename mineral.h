#pragma once
#include "object.h"

struct Mineral : Object
{
	Mineral(unsigned short e) : energy(e) {}

	ObjectColor Color() override
	{
		return { 0, 71, 181 };
	}

	Command Tic() override
	{
		return move_bottom;
	}

	unsigned short Die() override
	{
		return 0;
	}
	bool Dying() override
	{
		return false;
	}
	unsigned short energy = 100;
};