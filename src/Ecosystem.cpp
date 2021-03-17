#include "Ecosystem.h"
#include "Environment.h"
#include <SFML/Graphics.hpp>

Ecosystem::Ecosystem(
	unsigned int width,
	unsigned int height,
	unsigned short light_power,
	double light_coef,
	unsigned short max_organic_to_eat,
	unsigned short max_entities_to_eat,
	unsigned short max_energy,
	unsigned short max_hp
)
	: Environment(
		width, 
		height,
		light_power,
		light_coef,
		max_organic_to_eat,
		max_entities_to_eat,
		max_energy,
		max_hp)

	, view(view_settings::terrain) 
{}

unsigned int Ecosystem::GetMapWidth()
{
	return map_width;
}
unsigned int Ecosystem::GetMapHeight()
{
	return map_height;
}

RGBColor Ecosystem::ObtainEntityColor(EntitiesIterator entity)
{
	switch (view)
	{
	case view_settings::terrain:
		return { 13, 168, 19 };
	case view_settings::organic:
		return { 209, 209, 209 };
	case view_settings::ration:
		return entity->GetGenome().Ration();
	case view_settings::species:
		return entity->GetGenome().Species();
	case view_settings::energy:
	{
		if (entity->Energy() < (max_energy / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(255 * (entity->Energy() / (double)(max_energy / 2))), 
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(255 - 255 * (entity->Energy() / (double)max_energy)),
				255,
				0 
			};
		}
	}
	case view_settings::age:
	{
		unsigned char c = static_cast<unsigned char>(
			255 - 255 * ((double)entity->Age() / entity->MaxAge())
		);
		return { c, c, c };
	}
	case view_settings::hp:
	{
		if (entity->Hp() < (max_hp / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(255 * (entity->Hp() / (double)max_hp)),
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(255 - 255 * (entity->Hp() / (double)max_hp)), 
				255, 
				0 
			};
		}
	}
	case view_settings::success:
	{
		switch (Environment::SuccessRule(entity))
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
		unsigned char c = static_cast<unsigned char>(
			255 * ((double)entity->GetGenome().Generation()) / max_generation
		);
		return { c, c, c }; 
	}
	default:
		return {};
	}
}

RGBColor Ecosystem::ObtainColor(size_t x, size_t y)
{
	if (terrain[y][x].ContainsEntity() && view != view_settings::organic)
	{
		return ObtainEntityColor(terrain[y][x].GetEntity());
	}
	else if (terrain[y][x].IsContainsOrganic())
	{
		return	{ 0, 171, 209 };
	}

	return { 141, 219, 255 };
}

void Ecosystem::Draw(tgui::Canvas::Ptr canvas)
{
	canvas->clear({ 141, 219, 255 });

	sf::RectangleShape sprite;
	sprite.setOutlineThickness(0);
	sprite.setOutlineColor({ 0, 0, 0 });
	sf::Vector2f pos;
	sf::Vector2f size(cell_size, cell_size);
	sf::Vector2f osize(cell_outline, cell_outline);

	auto object = organic.begin();
	while (object != organic.end())
	{
		auto x = object->GetX();
		auto y = object->GetY();

		pos.x = x * cell_outline;
		pos.y = y * cell_outline;
		sprite.setPosition(pos);
		sprite.setSize(osize);

		if (observed_entity)
		{
			sprite.setFillColor({ 0, 171, 209, 100 });
		}
		else
		{
			sprite.setFillColor({ 0, 171, 209 });
		}
		canvas->draw(sprite);

		object++;
	}

	auto entity = entities.begin();
	if (view == view_settings::organic)
	{
		entity = entities.end();
	}

	sprite.setOutlineThickness(OUTLINE);
	while (entity != entities.end())
	{
		auto x = entity->GetX();
		auto y = entity->GetY();

		pos.x = x * ((size_t)cell_size + OUTLINE);
		pos.y = y * ((size_t)cell_size + OUTLINE);
		sprite.setPosition(pos);
		sprite.setSize(size);

		auto color = ObtainEntityColor(entity);
		if (observed_entity)
		{
			if (observed_entity->GetX() != x ||
				observed_entity->GetY() != y)
			{
				color.a = 100;
			}
		}
		sprite.setFillColor(color);

		canvas->draw(sprite);

		entity++;
	}

	canvas->display();
}

size_t Ecosystem::GetMaxGeneration()
{
	return max_generation;
}

void Ecosystem::ScaleCellSize(float scale)
{
	cell_size = cell_default_size * scale;
	cell_outline = cell_size + OUTLINE;
	map_width = width * cell_outline + 1;
	map_height = height * cell_outline + 1;
}

Info Ecosystem::GetInfoByCellsCoords(size_t x, size_t y)
{
	Info info;
	if (x >= width || y >= height)
	{
		return info;
	}

	info.color = ObtainColor(x, y);

	info.light_power = (((height - (double)y) / height) * light_coef) * light_power * 2;
	info.contains_entity = terrain[y][x].ContainsEntity();
	if (terrain[y][x].ContainsEntity())
	{
		observed_entity = *terrain[y][x].GetEntity();
		info.age.curr = terrain[y][x].GetEntity()->Age();
		info.age.max = terrain[y][x].GetEntity()->MaxAge();
		info.genome = terrain[y][x].GetEntity()->GetGenome().Data();
		info.genome_args = terrain[y][x].GetEntity()->GetGenome().Args();
		info.generation = terrain[y][x].GetEntity()->GetGenome().Generation();
		info.hp = terrain[y][x].GetEntity()->Hp();
		info.ch_of_mut = terrain[y][x].GetEntity()->GetGenome().MutationChance();
		info.energy = terrain[y][x].GetEntity()->Energy();
		info.max_energy = max_energy;
		info.max_hp = max_hp;
	}
	if (terrain[y][x].IsContainsOrganic())
	{
		info.food_power = terrain[y][x].GetOrganic()->Energy();
	}
	return info;
}
Info Ecosystem::GetInfoByPixelCoords(size_t x_px, size_t y_px)
{
	return GetInfoByCellsCoords(x_px / cell_outline, y_px / cell_outline);
}

void Ecosystem::Observing(Entity* entity)
{
	observed_entity = entity;
}
Entity* Ecosystem::Observing()
{
	return observed_entity;
}
size_t Ecosystem::GetEntitiesCount()
{
	return entities.size();
}
view_settings Ecosystem::GetView()
{
	return view;
}
void Ecosystem::SetView(view_settings new_val)
{
	view = new_val;
}