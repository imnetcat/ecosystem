#pragma once
class Item
{
protected:
	bool eatable;
	bool walkable;
public:
	Item(bool ea, bool w);

	Item& operator = (const Item& obj)
	{
		eatable = obj.eatable;
		walkable = obj.walkable;
		return *this;
	}
	Item& operator = (Item&& obj)
	{
		eatable = obj.eatable;
		walkable = obj.walkable;
		return *this;
	}

	void Walkable(bool val);
	bool IsWalkable();
	bool isEatable();
};
