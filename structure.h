#pragma once
#include "minerals.h"
#include "cell.h"
#include <memory>
#include <map>

class Structure
{
protected:
	RGBColor color;
	bool walkable;
	Cell cell;
	bool has_cell = false;
	Minerals food;
public:
	Structure() {}
	Structure(Structure&&) = default;
	Structure(const Structure&) = default;
	Structure(RGBColor c, bool w, float tr);

	Structure& operator = (const Structure& structure);
	Structure& operator = (Structure&& structure);

	void SetCell(const Cell& c);
	void SetCell();
	void DelCell();
	Cell& GetCell();
	bool ContainsCell() const;

	RGBColor Color();

	void SetFood(Minerals f);
	void SetFood(unsigned short energy);
	void DelFood();
	Minerals& GetFood();
	const Minerals& GetFood() const;

	void Walkable(bool val);
	bool IsWalkable();

	bool IsContainsFood();
};
