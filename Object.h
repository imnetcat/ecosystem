#pragma once
#include "genome.h"

class Object
{
protected:
	RGBColor color;
	bool walkable;
	float transparency;
	enum class GravityEffect {
		Down,
		Up,
		None
	} gravity;

public:
	Object(RGBColor color, bool walkable, float transparency, GravityEffect gravity);

	void Walkable(bool);
	bool Walkable();

	void Transparency(float);
	float Transparency();

	void Color(RGBColor);
	const RGBColor& Color();

	void Gravity(GravityEffect);
	GravityEffect Gravity();
};
