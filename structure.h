#pragma once
#include "object.h"
#include "minerals.h"
#include "cell.h"
#include <memory>
#include <map>

class Structure : public Object
{
protected:
	bool ticed;
	RGBColor color;
	const float transparency;
	unsigned char light_level;
	unsigned short light_power;
	bool walkable;
	Minerals food;
	Cell* cell;
public:
	Structure(RGBColor c, bool w, float tr);

	float Transparency();

	void Untick();

	RGBColor TerrainColor() override;
	RGBColor MineralsColor() override;
	RGBColor RationColor() override;
	RGBColor EnergyColor() override;
	RGBColor SpeciesColor() override;
	RGBColor AgeColor() override;
	RGBColor HpColor() override;
	RGBColor SurvivalColor() override;
	void Tic(MapTerrain& terrain, size_t& x, size_t& y) override;

	unsigned char GetLightLevel();
	void SetLightLevel(unsigned short lv);
	unsigned short GetLightPower();
	void SetLightPower(unsigned short lp);

	bool Outline(view_settings vs) override;
	
	void SetFood(Minerals f);
	void SetFood(unsigned short energy);
	void DelFood();
	Minerals& GetFood();
	const Minerals& GetFood() const;

	void SetCell(Cell* e);
	void DelCell();
	void CleanCell();
	Cell* GetCell();
	
	void Walkable(bool val);
	bool IsWalkable();

	bool IsContainsFood();
	bool IsContainsCell();
};
