#pragma once
#include <vector>
const short OUTLINE = 1;
const short CELL_SIZE = 10;
const short CELL_OUTLINE = CELL_SIZE + OUTLINE;

const unsigned short LIGHT_LEVEL_POWER = 10;
const unsigned int MAX_CELL_AGE = 50; // в тиках
const unsigned int MAX_ACC_ENERGY = 32000;
const unsigned int MAX_ENERGY = 100;
const unsigned int MAX_HP = 100;

enum view_settings
{
	default,
	energy,
	species
};

struct RGBColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};

struct Object
{
	virtual RGBColor Color(view_settings) = 0;
	virtual bool Outline() = 0;
};
