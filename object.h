#pragma once
#include "ration.h"

const short CELL_SIZE = 15;

enum Command
{
	move_left,
	move_right,
	move_bottom,
	move_top,
	skip,
	fotosintesis,
	furcation,  // деление на две
};

struct ObjectColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};


struct Object
{
	virtual void IncreaceEnergy(unsigned short value) = 0;
	virtual Command Tic() = 0;
	virtual ObjectColor Color() = 0;
};
