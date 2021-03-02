#pragma once
#include "minerals.h"
#include "entity.h"
#include "Material.h"
#include <memory>
#include <map>

class TerrainCell
{
protected:
	Minerals food;
	Entity entity;
	Material material;
	bool has_entity = false;
public:
	TerrainCell(Material material) : material(material) {}

	void SetEntity(const Entity& c);
	void SetEntity();
	void DelEntity();
	Entity& GetEntity();
	bool ContainsEntity() const;

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
