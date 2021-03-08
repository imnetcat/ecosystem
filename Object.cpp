#include "Object.h"
#include "config.h"

Object::Object() :
	x(0),
	y(0)
{ }

Object::Object(size_t x, size_t y) :
	x(x),
	y(y)
{ }

size_t Object::GetX() const
{
	return x;
}
size_t Object::GetY() const
{
	return y;
}
void Object::SetX(size_t value)
{
	x = value;
}
void Object::SetY(size_t value)
{
	y = value;
}
void Object::SetPosition(size_t nx, size_t ny)
{
	x = nx;
	y = ny;
}
