#include "position.h"
using namespace ecosystem::logic;

position::position()
	: x_(0)
	, y_(0)
{ }

position::position(int x, int y)
	: x_(x)
	, y_(y)
{ }

int position::x() const
{
	return x_;
}
int position::y() const
{
	return y_;
}
void position::x(int value)
{
	x_ = value;
}
void position::y(int value)
{
	y_ = value;
}
void position::set(int x, int y)
{
	x_ = x;
	y_ = y;
}
