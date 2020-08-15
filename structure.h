#pragma once
#include "object.h"
#include "food.h"
#include "entity.h"
#include <memory>
#include <map>

class Structure : public Object
{
protected:
	bool ticed;
	RGBColor color;
	const double transparency;
	unsigned char light_level;
	unsigned short light_power;
	bool walkable;
	bool default_walkable;
	Food food;
	std::shared_ptr<Entity> entity;
	bool contains_entity;
public:
	Structure(RGBColor c, bool w, unsigned short lv, double tr);

	double Transparency();

	void Untick();

	RGBColor Color() override;

	void Tic(std::vector<Command>& commands) override;

	unsigned short GetLightLevel();
	void SetLightLevel(unsigned short lv);
	unsigned short GetLightPower();
	void SetLightPower(unsigned short lp);

	bool Outline() override;
	
	void SetFood(Food f);
	void SetFood(ration r, unsigned short energy);
	void DelFood();
	Food& GetFood();
	const Food& GetFood() const;

	void SetEntity(std::shared_ptr<Entity> e);
	void DelEntity();
	std::shared_ptr<Entity>& GetEntity();
	
	void Walkable(bool val);
	bool IsWalkable();

	bool IsContainsFood();
	bool IsContainsEntity();
};
