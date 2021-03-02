#include "Object.h"
#include "config.h"

Object::Object(RGBColor color, bool walkable, float transparency, GravityEffect gravity) :
	color(color),
	walkable(walkable),
	transparency(transparency),
	gravity(gravity)
{ }

void Object::Walkable(bool value)
{
	walkable = value;
}
bool Object::Walkable()
{
	return walkable;
}

void Object::Transparency(float value)
{
	transparency = value;
}
float Object::Transparency()
{
	return transparency;
}

void Object::Color(RGBColor value)
{
	color = value;
}
const RGBColor& Object::Color()
{
	return color;
}

void Object::Gravity(GravityEffect value)
{
	gravity = value;
}
Object::GravityEffect Object::Gravity()
{
	return gravity;
}
