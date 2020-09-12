#pragma once
#include <vector>
const short OUTLINE = 1;
const short CELL_SIZE = 4;
const short CELL_OUTLINE = CELL_SIZE + OUTLINE;

const unsigned short LIGHT_POWER = 10000;
const unsigned short MAX_LIGHT_TO_EAT = 5000;
const unsigned short MAX_MINERALS_TO_EAT = 5000;
const unsigned int MAX_ACC_ENERGY = 64000;
const unsigned int MAX_ENERGY = 100;
const unsigned int MAX_HP = 100;

enum view_settings
{
	terrain,
	minerals,
	ration,
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
	virtual bool Outline(view_settings) = 0;
};
