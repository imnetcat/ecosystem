#include "Ecosystem.h"
#include "config.h"

static array<array<sf::RectangleShape, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> sprites;

Ecosystem::Ecosystem()
{
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			// init map sprites positions
			sf::RectangleShape cell;
			sf::Vector2f vec;
			vec.x = x * CELL_OUTLINE;
			vec.y = y * CELL_OUTLINE;
			cell.setPosition(vec);
			sprites[y][x] = cell;
		}
	}
}

RGBColor Ecosystem::ObtainColor(size_t x, size_t y)
{
	RGBColor color;
	if (terrain[y][x].ContainsCell() && view != view_settings::minerals)
	{
		switch (view)
		{
		case view_settings::terrain:
			return { 13, 168, 19 };
			break;
		case view_settings::minerals:
			return { 209, 209, 209 };
			break;
		case view_settings::ration:
			color = terrain[y][x].GetCell().RationColor();
			break;
		case view_settings::energy:
		{
			if (terrain[y][x].GetCell().Energy() < (MAX_ENERGY / 2))
			{
				return { 255, static_cast<unsigned char>(255 * (terrain[y][x].GetCell().Energy() / (double)(MAX_ENERGY / 2))), 0 };
			}
			else
			{
				return { static_cast<unsigned char>(255 - 255 * (terrain[y][x].GetCell().Energy() / (double)MAX_ENERGY)), 255, 0 };
			}
		}
		case view_settings::species:
			return terrain[y][x].GetCell().Species();
		case view_settings::age:
		{
			unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)terrain[y][x].GetCell().Age() / terrain[y][x].GetCell().MaxAge()));
			return { c, c, c };
		}
		case view_settings::hp:
		{
			if (terrain[y][x].GetCell().Hp() < (MAX_HP / 2))
			{
				return { 191, static_cast<unsigned char>(191 * (terrain[y][x].GetCell().Hp() / (double)(MAX_HP / 2))), 0 };
			}
			else
			{
				return { static_cast<unsigned char>(191 * ((double)(MAX_HP / 2) / terrain[y][x].GetCell().Hp())), 191, 0 };
			}
		}
		case view_settings::survival:
		{
			switch (terrain[y][x].GetCell().SuccessRule())
			{
			case Cell::Success::fail:
				return { 255, 21, 0 };
				break;
			case Cell::Success::normal:
				return { 255, 225, 0 };
				break;
			case Cell::Success::good:
				return { 0, 194, 0 };
				break;
			default:
				return { 255, 225, 0 };
				break;
			}
		}
		case view_settings::generations:
		{
			unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)terrain[y][x].GetCell().GetGenome().generation / max_generation));
			return { c, c, c }; }
		}
	}
	else if (!terrain[y][x].GetFood().Empty())
	{
		return	{ 0, 171, 209 };
	}
	else
	{
		return terrain[y][x].Color();
	}
}

void Ecosystem::Draw(sf::RenderWindow& window)
{
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			RGBColor color = ObtainColor(x, y);

			if (terrain[y][x].ContainsCell() && view != view_settings::minerals)
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

Info Ecosystem::GetInfo(size_t x_px, size_t y_px)
{
	size_t x = x_px / CELL_OUTLINE;
	size_t y = y_px / CELL_OUTLINE;
	Info info;

	info.color = ObtainColor(x, y);

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

view_settings Ecosystem::GetView()
{
	return view;
}
void Ecosystem::SetView(view_settings new_val)
{
	view = new_val;
}