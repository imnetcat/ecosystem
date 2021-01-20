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
	std::vector<int> genom;
	unsigned short hp = 0;
	size_t energy = 0;
	size_t generation = 0;
	size_t light_power = 0;
	size_t food_power = 0;
	float ch_of_mut;
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
	view_settings view = view_settings::terrain;
};
