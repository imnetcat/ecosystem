#pragma once
#include "config.h"
#include <map>

enum class view_side
{
	top,
	right_top,
	right,
	right_bottom,
	bottom,
	left_bottom,
	left,
	left_top,
};

enum class view_settings
{
	terrain,
	minerals,
	ration,
	energy,
	species,
	age,
	hp,
	survival,
	generations
};

struct RGBColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};

struct Object
{
	virtual RGBColor GenerationsColor() = 0;
	virtual RGBColor TerrainColor() = 0;
	virtual RGBColor MineralsColor() = 0;
	virtual RGBColor RationColor() = 0;
	virtual RGBColor EnergyColor() = 0;
	virtual RGBColor SpeciesColor() = 0;
	virtual RGBColor AgeColor() = 0;
	virtual RGBColor HpColor() = 0;
	virtual RGBColor SurvivalColor() = 0;
	virtual bool Outline(view_settings) = 0;
};
