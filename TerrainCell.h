#pragma once
#include "Water.h"
#include "Organic.h"
#include "Entity.h"
#include "Material.h"
#include <memory>
#include <map>

class TerrainCell
{
protected:
	size_t organic;
	Entity entity;
	Material material;
	bool has_entity = false;
public:
	TerrainCell() : material(Water()) {}

	void SetEntity();
	void DelEntity();
	Entity& GetEntity();
	bool ContainsEntity() const;

	RGBColor Color();

	void SetFood(size_t organic);
	void DelFood();
	size_t GetFood() const;

	void Walkable(bool val);
	bool IsWalkable();

	bool IsContainsFood();
};
