#include "Ecosystem.h"
#include "Environment.h"
#include "config.h"

static array<array<sf::RectangleShape, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> sprites;

Ecosystem::Ecosystem()
	: view(view_settings::terrain)
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
	if (terrain[y][x].ContainsEntity() && view != view_settings::minerals)
	{
		switch (view)
		{
		case view_settings::terrain:
			return { 13, 168, 19 };
		case view_settings::minerals:
			return { 209, 209, 209 };
		case view_settings::ration:
		{
			Ration ration = terrain[y][x].GetEntity()->GetGenome().Ration();

			switch (ration)
			{
			case Ration::entities:
				return { 255, 51, 51 };
			case Ration::light:
				return { 255, 245, 30 };
			case Ration::organic:
				return { 37, 53, 217 };
			case Ration::entites_organic:
				return { 50, 255, 30 };
			case Ration::entites_light:
				return { 255, 160, 30 };
			case Ration::light_organic:
				return { 30, 255, 248 };
			case Ration::omnivorous:
				return { 0, 0, 0 };
			default:
				return { 0, 0, 0 };
			}
			break;
		}
		case view_settings::energy:
		{
			if (terrain[y][x].GetEntity()->Energy() < (MAX_ENERGY / 2))
			{
				return { 255, static_cast<unsigned char>(255 * (terrain[y][x].GetEntity()->Energy() / (double)(MAX_ENERGY / 2))), 0 };
			}
			else
			{
				return { static_cast<unsigned char>(255 - 255 * (terrain[y][x].GetEntity()->Energy() / (double)MAX_ENERGY)), 255, 0 };
			}
		}
		case view_settings::species:
			return terrain[y][x].GetEntity()->GetGenome().Species();
		case view_settings::age:
		{
			unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)terrain[y][x].GetEntity()->Age() / terrain[y][x].GetEntity()->MaxAge()));
			return { c, c, c };
		}
		case view_settings::hp:
		{
			if (terrain[y][x].GetEntity()->Hp() < (MAX_HP / 2))
			{
				return { 191, static_cast<unsigned char>(191 * (terrain[y][x].GetEntity()->Hp() / (double)(MAX_HP / 2))), 0 };
			}
			else
			{
				return { static_cast<unsigned char>(191 * ((double)(MAX_HP / 2) / terrain[y][x].GetEntity()->Hp())), 191, 0 };
			}
		}
		case view_settings::survival:
		{
			switch (Environment::SuccessRule(terrain[y][x].GetEntity()))
			{
			case Coefficient::reduce:
				return { 255, 21, 0 };
				break;
				case Coefficient::unchanged:
				return { 255, 225, 0 };
				break;
				case Coefficient::enlarge:
				return { 0, 194, 0 };
				break;
			default:
				return { 255, 225, 0 };
				break;
			}
		}
		case view_settings::generations:
		{
			unsigned char c = static_cast<unsigned char>(255 * ((double)terrain[y][x].GetEntity()->GetGenome().Generation()) / max_generation);
			return { c, c, c }; }
		}
	}
	else if (terrain[y][x].GetFood())
	{
		return	{ 0, 171, 209 };
	}

	return { 141, 219, 255 };
}

void Ecosystem::Draw(sf::RenderWindow& window)
{
	for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
	{
		for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
		{
			RGBColor color = ObtainColor(x, y);

			if (terrain[y][x].ContainsEntity() && view != view_settings::minerals)
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
	if (terrain[y][x].ContainsEntity())
	{
		info.age.curr = terrain[y][x].GetEntity()->Age();
		info.age.max = terrain[y][x].GetEntity()->MaxAge();
		info.genome = terrain[y][x].GetEntity()->GetGenome().Data();
		info.generation = terrain[y][x].GetEntity()->GetGenome().Generation();
		info.hp = terrain[y][x].GetEntity()->Hp();
		info.ch_of_mut = terrain[y][x].GetEntity()->GetGenome().MutationChance();
		info.energy = terrain[y][x].GetEntity()->Energy();
	}
	if (terrain[y][x].IsContainsFood())
	{
		info.food_power = terrain[y][x].GetFood();
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