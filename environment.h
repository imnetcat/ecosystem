#pragma once

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
#include <cmath>
#include "water.h"
#include "air.h"

#include <memory>
#include <ctime>
#include <array>

#include "sfml.h"
#include "map_terrain.h"

using namespace std;

const int CELL_START_COUNT = 10;

#define GLASS std::shared_ptr<Structure>(new Glass())
#define WATER std::shared_ptr<Structure>(new Water())
#define AIR std::shared_ptr<Structure>(new Air())
#define CERAMIC std::shared_ptr<Structure>(new Ceramic())
#define EARTH std::shared_ptr<Structure>(new Earth())
#define SAND std::shared_ptr<Structure>(new Sand())

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

static array<sf::RectangleShape, ENVIRONMENT_SIZE_X* ENVIRONMENT_SIZE_Y> sprites;

class Environment
{
public:
	explicit Environment()
	{
		size_t index = 0;
		size_t count = 0;
		srand(time(0) + rand());
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				// set up looped body of water

				terrain[y][x] = shared_ptr<Structure>(new Water());

				// put minerals
				if (((rand() % 100) < 50))
				{
					terrain[y][x]->SetFood(2500);
				}

				// put first cells
				if (((rand() % 1000) < 1) && count < CELL_START_COUNT)
				{
					count++;
					terrain[y][x]->SetCell(
						new Cell(
							0,
							150,
							200,
							100,
							0.01,
							25,
							Genome(),
							ORGANELLES
						)
					);
				}


				// set up light
				terrain[y][x]->SetLightPower(LIGHT_POWER);

				// init map sprites positions
				sf::RectangleShape cell;
				sf::Vector2f vec;
				vec.x = x * CELL_OUTLINE;
				vec.y = y * CELL_OUTLINE;
				cell.setPosition(vec);
				sprites[index] = cell;

				index++;
			}
		}
	}
	
	void Update(sf::RenderWindow& window, bool update_env)
	{
		size_t index = 0;
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				if (update_env)
				{

					// Check if terrain contains dead cell
					bool contains_dead_cell = false;
					if (terrain[y][x]->IsContainsCell())
					{
						contains_dead_cell = terrain[y][x]->GetCell()->IsDead();
					}

					// delete cell if terrain contains dead cell
					if (contains_dead_cell)
					{
						if (terrain[y][x]->IsContainsFood())
							terrain[y][x]->GetFood().Put(terrain[y][x]->GetCell()->Energy() + 100);
						else
							terrain[y][x]->SetFood(terrain[y][x]->GetCell()->Energy() + 100);

						terrain[y][x]->DelCell();
					}
					size_t changable_x = x;
					size_t changable_y = y;
					terrain[y][x]->Tic(terrain, changable_x, changable_y);

					terrain[y][x]->Untick();
				}

				RGBColor color;
				switch (view)
				{
				case view_settings::terrain:
					color = terrain[y][x]->TerrainColor();
					break;
				case minerals:
					color = terrain[y][x]->MineralsColor();
					break;
				case ration:
					color = terrain[y][x]->RationColor();
					break;
				case energy:
					color = terrain[y][x]->EnergyColor();
					break;
				case species:
					color = terrain[y][x]->SpeciesColor();
					break;
				case age:
					color = terrain[y][x]->AgeColor();
					break;
				case hp:
					color = terrain[y][x]->HpColor();
					break;
				case survival:
					color = terrain[y][x]->SurvivalColor();
					break;
				}

				if(terrain[y][x]->Outline(view))
				{
					sprites[index].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
					sprites[index].setOutlineThickness(OUTLINE);
				}
				else
				{
					sprites[index].setSize(sf::Vector2f(CELL_OUTLINE, CELL_OUTLINE));
					sprites[index].setOutlineThickness(0);
				}

				sprites[index].setOutlineColor({ 0, 0, 0 });
				sprites[index].setFillColor({ color.r, color.g, color.b });
				
				window.draw(sprites[index]);

				index++;
			}
		}
	}

	Info GetInfo(size_t x_px, size_t y_px)
	{
		size_t x = x_px / CELL_OUTLINE;
		size_t y = y_px / CELL_OUTLINE;
		Info info;

		switch (view)
		{
		case view_settings::terrain:
			info.color = terrain[y][x]->TerrainColor();
			break;
		case minerals:
			info.color = terrain[y][x]->MineralsColor();
			break;
		case ration:
			info.color = terrain[y][x]->RationColor();
			break;
		case energy:
			info.color = terrain[y][x]->EnergyColor();
			break;
		case species:
			info.color = terrain[y][x]->SpeciesColor();
			break;
		case age:
			info.color = terrain[y][x]->AgeColor();
			break;
		case hp:
			info.color = terrain[y][x]->HpColor();
			break;
		case survival:
			info.color = terrain[y][x]->SurvivalColor();
			break;
		}

		info.light_power = terrain[y][x]->GetLightPower();
		if (terrain[y][x]->IsContainsCell())
		{
			info.age.curr = terrain[y][x]->GetCell()->Age();
			info.age.max = terrain[y][x]->GetCell()->MaxAge();
			auto& data = terrain[y][x]->GetCell()->GetGenome().data;
			for (unsigned int i = 0; i < data.size(); i++)
			{
				info.genom.push_back(static_cast<int>(data[i]));
			}
			info.generation = terrain[y][x]->GetCell()->GetGenome().generation;
			info.hp = terrain[y][x]->GetCell()->Hp();
			info.ch_of_mut = terrain[y][x]->GetCell()->GetGenome().mutationChance;
			info.energy = terrain[y][x]->GetCell()->Energy();
		}
		if (terrain[y][x]->IsContainsFood())
		{
			info.food_power = terrain[y][x]->GetFood().Get();
		}
 		return info;
	}

	view_settings GetView()
	{
		return view;
	}
	void SetView(view_settings new_val)
	{
		view = new_val;
	}
private:

	view_settings view = view_settings::terrain;
	
	MapTerrain terrain;
};
