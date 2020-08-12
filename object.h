#pragma once

const short CELL_SIZE = 10;

enum Command
{
	move_left,
	move_right,
	move_bottom,
	move_top,
	skip,
};

struct ObjectColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};


struct Object
{
	virtual Command Tic() = 0;
	virtual ObjectColor Color() = 0;
	virtual unsigned short Die() = 0;
	virtual bool Dying() = 0;
};
