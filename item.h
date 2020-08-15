#pragma once
#include "object.h"

class Item : public Object
{
protected:
	const bool eatable;
	bool walkable;
public:
	Item(bool ea, bool w);

	void Walkable(bool val);
	bool IsWalkable();
	bool isEatable();
};
