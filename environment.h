#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
#include <cmath>
#include "water.h"

#include <memory>
#include <ctime>
#include <array>

#include "sfml.h"
#include "map_terrain.h"
#include "cell.h"

using namespace std;

using CellId = size_t;
using ObjectId = size_t;
using PositionId = size_t;

struct DrawData
{
	Position position;
	RGBColor color;
	bool outline;
	unsigned char shadow;
};

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

struct LightLevel
{
	size_t power;
	size_t power_max;
};

static array<array<sf::RectangleShape, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> sprites;

class Environment
{
public:
	explicit Environment();
	void Update();
	void Draw(sf::RenderWindow& window);
	Info GetInfo(size_t x_px, size_t y_px);
	view_settings GetView();
	void SetView(view_settings new_val);
private:
	view_settings view = view_settings::terrain;
};
