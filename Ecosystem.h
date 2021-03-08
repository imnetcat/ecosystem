#pragma once
#include "environment.h"

struct Info
{
	RGBColor color;
	struct Age
	{
		size_t curr = 0;
		size_t max = 0;
	} age;
	bool contains_entity;
	unsigned __int64 genome;
	unsigned short hp = 0;
	size_t energy = 0;
	size_t generation = 0;
	size_t light_power = 0;
	size_t food_power = 0;
	unsigned __int8 ch_of_mut;
};

class Ecosystem : public Environment
{
public:
	Ecosystem();
	void Draw(sf::RenderWindow& window);
	Info GetInfo(size_t x_px, size_t y_px);
	view_settings GetView();
	void SetView(view_settings new_val);
private:
	RGBColor ObtainColor(size_t x, size_t y);
	view_settings view;
};
