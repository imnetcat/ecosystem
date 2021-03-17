#pragma once
#include "logic/world.h"
#include <TGUI/TGUI.hpp>

struct Info
{
	RGBColor color;
	struct Age
	{
		size_t curr = 0;
		size_t max = 0;
	} age;
	bool contains_entity = 0;
	unsigned __int64 genome = 0;
	unsigned __int8 genome_args = 0;
	unsigned short hp = 0;
	unsigned short max_hp = 0;
	size_t energy = 0;
	size_t max_energy = 0;
	size_t generation = 0;
	size_t light_power = 0;
	size_t food_power = 0;
	unsigned __int8 ch_of_mut = 0;
};

class Map
{
public:
	Map(World* world);

	void Draw(tgui::Canvas::Ptr canvas);
	RGBColor ObtainEntityColor(pool<Entity>::const_iterator entity);
	RGBColor ObtainColor(size_t x, size_t y);

	view_settings GetView();
	void SetView(view_settings new_val);

	void ScaleCellSize(float scale);
	unsigned int Width();
	unsigned int Height();
	size_t GetMaxGeneration();
	size_t GetEntitiesCount();

	Info GetInfoByCellsCoords(size_t x, size_t y);
	Info GetInfoByPixelCoords(size_t x_px, size_t y_px);

private:

	World* world;

	const short OUTLINE = 1;

	static const unsigned short cell_default_size = 10;
	unsigned short cell_size = cell_default_size;
	unsigned short cell_outline = cell_size + OUTLINE;
	unsigned int map_width = world->Width() * cell_outline + 1;
	unsigned int map_height = world->Height() * cell_outline + 1;

	view_settings view = view_settings::terrain;
};
