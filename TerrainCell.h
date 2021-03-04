#pragma once
#include "Water.h"
#include "Organic.h"
#include "Entity.h"
#include "Material.h"
#include "config.h"
#include <memory>
#include <map>
#include <array>

class TerrainCell
{
protected:
	size_t organic;
	EntitiesIterator entity;
	Material material;
	bool has_entity = false;
public:
	TerrainCell() : organic(0), material(Water()) {}

	void SetEntity(EntitiesIterator);
	void DelEntity();
	EntitiesIterator GetEntity();
	bool ContainsEntity() const;

	void SetFood(size_t organic);
	void DelFood();
	size_t GetFood() const;

	bool IsWalkable();

	bool IsContainsFood();
};
