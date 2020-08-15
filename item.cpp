#include "item.h"

Item::Item(bool ea, bool w) :
	walkable(w),
	eatable(ea) {}

void Item::Walkable(bool val)
{
	walkable = val;
}
bool Item::IsWalkable()
{
	return walkable;
}
bool Item::isEatable()
{
	return eatable;
}
