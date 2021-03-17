#include "position.h"

position::position()
	: x_(0)
	, y_(0)
{ }

position::position(size_t x, size_t y)
	: x_(x)
	, y_(y)
{ }

size_t position::x() const
{
	return x_;
}
size_t position::y() const
{
	return y_;
}
void position::x(size_t value)
{
	x_ = value;
}
void position::y(size_t value)
{
	y_ = value;
}
void position::SetPosition(size_t x, size_t y)
{
	x_ = x;
	y_ = y;
}

bool operator == (const position& lhs, const position& rhs)
{
	return lhs.x_ == rhs.x_ && lhs.y_ == rhs.y_;
}
