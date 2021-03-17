#pragma once
#include "environment.h"
#include <TGUI/TGUI.hpp>

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
	unsigned __int8 genome_args;
	unsigned short hp = 0;
	unsigned short max_hp = 0;
	size_t energy = 0;
	size_t max_energy = 0;
	size_t generation = 0;
	size_t light_power = 0;
	size_t food_power = 0;
	unsigned __int8 ch_of_mut;
};

class Ecosystem : public Environment
{
public:
	Ecosystem(
		unsigned int width,
		unsigned int height,
		unsigned short light_power,
		double light_coef,
		unsigned short max_organic_to_eat,
		unsigned short max_entities_to_eat,
		unsigned short max_energy,
		unsigned short max_hp
	);

	void Draw(tgui::Canvas::Ptr canvas);
	Info GetInfoByCellsCoords(size_t x, size_t y);
	Info GetInfoByPixelCoords(size_t x_px, size_t y_px);
	view_settings GetView();
	void SetView(view_settings new_val);
	void Observing(Entity* entity);
	Entity* Observing();
	RGBColor ObtainEntityColor(EntitiesIterator entity);
	RGBColor ObtainColor(size_t x, size_t y);

	void ScaleCellSize(float scale);
	unsigned int GetMapWidth();
	unsigned int GetMapHeight();
	size_t GetMaxGeneration();
	size_t GetEntitiesCount();

private:

	const short OUTLINE = 1;

	static const unsigned short cell_default_size = 10;
	unsigned short cell_size = cell_default_size;
	unsigned short cell_outline = cell_size + OUTLINE;
	unsigned int map_width = width * cell_outline + 1;
	unsigned int map_height = height * cell_outline + 1;
	
	view_settings view;
};
