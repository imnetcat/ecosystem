#pragma once
#include <vector>
const short OUTLINE = 1;
const short CELL_SIZE = 15;
const short CELL_OUTLINE = CELL_SIZE + OUTLINE;

const unsigned short LIGHT_LEVEL_POWER = 10;
const unsigned int MAX_CELL_AGE = 100; // в тиках
const unsigned short ENERGY_PER_TIC = 5; // в тиках

enum Command
{
	gravity,
	die,
	move_left,
	move_right,
	move_bottom,
	move_top,
	skip,
	eat,
	furcation,  // деление на две
};

struct RGBColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};

struct Object
{
	virtual void Tic(std::vector<Command>& commands) = 0;
	virtual RGBColor Color() = 0;
	virtual bool Outline() = 0;
};
