#pragma once
#include "object.h"
#include "minerals.h"
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
	Minerals food;
	std::shared_ptr<Entity> entity;
	bool contains_entity;
public:
	Structure(RGBColor c, bool w, double tr);

	double Transparency();

	void Untick();

	RGBColor Color(view_settings vs);

	void Tic(std::vector<Command>& commands);

	unsigned short GetLightLevel();
	void SetLightLevel(unsigned short lv);
	unsigned short GetLightPower();
	void SetLightPower(unsigned short lp);

	bool Outline() override;
	
	void SetFood(Minerals f);
	void SetFood(unsigned short energy);
	void DelFood();
	Minerals& GetFood();
	const Minerals& GetFood() const;

	void SetEntity(std::shared_ptr<Entity> e);
	void DelEntity();
	std::shared_ptr<Entity>& GetEntity();
	
	void Walkable(bool val);
	bool IsWalkable();

	bool IsContainsFood();
	bool IsContainsEntity();
};
