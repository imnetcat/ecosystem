#pragma once
#include "object.h"
#include "entity.h"
#include <memory>

class Structure : public Object
{
protected:
	bool ticed;
	bool gravityOn;
	RGBColor color;
	const bool eatable;
	const float transparency;
	unsigned short light_level;
	bool walkable;
	bool default_walkable;
	std::shared_ptr<Structure> structure;
	std::shared_ptr<Entity> entity;
	bool contains_struct;
	bool contains_entity;
public:
	Structure(RGBColor c, bool g, bool w, bool ea, unsigned short lv, float tr) :
		ticed(false),
		gravityOn(g),
		color(c),
		eatable(ea),
		walkable(w),
		transparency(tr),
		light_level(lv),
		default_walkable(walkable),
		contains_struct(false),
		contains_entity(false)
	{ }

	void Untick()
	{
		ticed = false;
		if (contains_struct)
			structure->Untick();
	}

	RGBColor Color() override
	{
		if (contains_entity)
			return entity->Color();

		if (contains_struct)
			return structure->Color();

		return color;
	}

	void Tic(std::vector<Command>& commands) override
	{
		if (ticed)
			return;

		if (gravityOn)
			commands.push_back(gravity);
		if(contains_struct)
			structure->Tic(commands);
		if (contains_entity)
			entity->Tic(commands);
		ticed = true;
	}

	bool isEatable()
	{
		return eatable;
	}
	unsigned short GetLightLevel()
	{
		return light_level;
	}
	void SetLightLevel(unsigned short lv)
	{
		light_level = lv;
	}

	bool Outline() override
	{
		if (contains_entity)
			return entity->Outline();
		if (contains_struct)
			return structure->Outline();

		return false;
	}

	void SetStruct(std::shared_ptr<Structure> obj)
	{
		structure = obj;
		structure->SetLightLevel(light_level);
		contains_struct = true;
		walkable = structure->IsWalkable();
	}
	void DelStruct()
	{
		structure = nullptr;
		contains_struct = false;
		walkable = default_walkable;
	}
	std::shared_ptr<Structure>& GetStruct()
	{
		return structure;
	}

	void SetEntity(std::shared_ptr<Entity> e)
	{
		entity = e;
		contains_entity = true;
		walkable = false;
	}
	void DelEntity()
	{
		entity = nullptr;
		contains_entity = false;
		walkable = default_walkable;
	}
	std::shared_ptr<Entity>& GetEntity()
	{
		return entity;
	}
	
	void Walkable(bool val) 
	{
		walkable = val;
	}
	bool IsWalkable()
	{
		return walkable;
	}

	bool IsContainsStruct()
	{
		return contains_struct;
	}
	bool IsContainsEntity()
	{
		return contains_entity;
	}
};
