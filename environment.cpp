#include "environment.h"

// organelles
#include "map_terrain.h"
#include "separationing.h"
#include "carnivorousing.h"
#include "mineraling.h"
#include "birthing.h"
#include "photosynthesing.h"
#include "moving.h"
#include "staying.h"
#include "turning.h"
static const std::map<Trigger, Organelle> ORGANELLES = {
	{Trigger::Stay, Staying},

	{Trigger::Birth, Birthing},
	{Trigger::Separate, Separationing},

	{Trigger::Photosyntesis, Photosynthesing},
	{Trigger::Carnivorous, Carnivorousing},
	{Trigger::Mineraleon, Mineraling},

	{Trigger::Move, Moving},
	{Trigger::Turn, Turning}
};

static array<array<sf::RectangleShape, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> sprites;

void Environment::Draw(sf::RenderWindow& window)
{
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			RGBColor color;
			switch (view)
			{
			case view_settings::terrain:
				color = terrain[y][x].TerrainColor();
				break;
			case view_settings::minerals:
				color = terrain[y][x].MineralsColor();
				break;
			case view_settings::ration:
				color = terrain[y][x].RationColor();
				break;
			case view_settings::energy:
				color = terrain[y][x].EnergyColor();
				break;
			case view_settings::species:
				color = terrain[y][x].SpeciesColor();
				break;
			case view_settings::age:
				color = terrain[y][x].AgeColor();
				break;
			case view_settings::hp:
				color = terrain[y][x].HpColor();
				break;
			case view_settings::survival:
				color = terrain[y][x].SurvivalColor();
				break;
			case view_settings::generations:
				color = terrain[y][x].GenerationsColor();
				break;
			}

			if (terrain[y][x].Outline(view))
			{
				sprites[y][x].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
				sprites[y][x].setOutlineThickness(OUTLINE);
			}
			else
			{
				sprites[y][x].setSize(sf::Vector2f(CELL_OUTLINE, CELL_OUTLINE));
				sprites[y][x].setOutlineThickness(0);
			}

			sprites[y][x].setOutlineColor({ 0, 0, 0 });
			sprites[y][x].setFillColor({ color.r, color.g, color.b });

			window.draw(sprites[y][x]);
		}
	}
}

Environment::Environment()
{
	size_t index = 0;
	srand(time(0) + rand());
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			// fill of water
			terrain[y][x] = Water();
			// fill of nullptr
			cells[index] = nullptr;

			// put minerals
			if (((rand() % 100) < 25))
			{
				terrain[y][x].SetFood(100);
			}

			// put first cells
			if (((rand() % 100) < 1) && cells_count < CELL_START_COUNT)
			{
				terrain[y][x].SetCell();
				cells[cells_count] = &terrain[y][x].GetCell();
				cells[cells_count]->SetPosition(x, y);
				cells[cells_count]->Energy(100);
				cells[cells_count]->MaxAge(100);
				cells[cells_count]->Defence(0.01);
				cells[cells_count]->Hp(MAX_HP);
				cells[cells_count]->Attack(0.01);
				cells[cells_count]->SetGenome(Genome());

				size_t cost = 0;
				for (const Gen& gen : cells[cells_count]->GetGenome().data)
				{
					cost += CREATION_COST.at(gen.trigger);
				}
				cells[cells_count]->ReproductionCost(100 + cost);

				cells_count++;
			}

			// init map sprites positions
			sf::RectangleShape cell;
			sf::Vector2f vec;
			vec.x = x * CELL_OUTLINE;
			vec.y = y * CELL_OUTLINE;
			cell.setPosition(vec);
			sprites[y][x] = cell;

			index++;
		}
	}
}

void Environment::Update()
{
	for (size_t index = 0; index < cells.size(); index++)
	{
		if (!cells[index]) break;

		size_t x = cells[index]->GetX();
		size_t y = cells[index]->GetY();
		if (cells[index]->IsDead())
			// remove cell if it is dead
		{
			CellDie(index, x, y);
		}
		else
		{
			cells[index]->Tic();

			Gen& gen = cells[index]->GetGenome().Read();
			cells[index]->DecreaceEnergy(MAINTENANACE_COST.at(gen.trigger));
			ORGANELLES.at(gen.trigger)(gen.args, x, y);
		}
	}
}

Info Environment::GetInfo(size_t x_px, size_t y_px)
{
	size_t x = x_px / CELL_OUTLINE;
	size_t y = y_px / CELL_OUTLINE;
	Info info;

	switch (view)
	{
	case view_settings::terrain:
		info.color = terrain[y][x].TerrainColor();
		break;
	case view_settings::minerals:
		info.color = terrain[y][x].MineralsColor();
		break;
	case view_settings::ration:
		info.color = terrain[y][x].RationColor();
		break;
	case view_settings::energy:
		info.color = terrain[y][x].EnergyColor();
		break;
	case view_settings::species:
		info.color = terrain[y][x].SpeciesColor();
		break;
	case view_settings::age:
		info.color = terrain[y][x].AgeColor();
		break;
	case view_settings::hp:
		info.color = terrain[y][x].HpColor();
		break;
	case view_settings::survival:
		info.color = terrain[y][x].SurvivalColor();
		break;
	}

	info.light_power = LIGHT_POWER;
	if (terrain[y][x].ContainsCell())
	{
		info.age.curr = terrain[y][x].GetCell().Age();
		info.age.max = terrain[y][x].GetCell().MaxAge();
		auto& data = terrain[y][x].GetCell().GetGenome().data;
		for (unsigned int i = 0; i < data.size(); i++)
		{
			info.genom.push_back(static_cast<int>(data[i].trigger));
		}
		info.generation = terrain[y][x].GetCell().GetGenome().generation;
		info.hp = terrain[y][x].GetCell().Hp();
		info.ch_of_mut = terrain[y][x].GetCell().GetGenome().mutationChance;
		info.energy = terrain[y][x].GetCell().Energy();
	}
	if (terrain[y][x].IsContainsFood())
	{
		info.food_power = terrain[y][x].GetFood().Get();
	}
	return info;
}

view_settings Environment::GetView()
{
	return view;
}
void Environment::SetView(view_settings new_val)
{
	view = new_val;
}