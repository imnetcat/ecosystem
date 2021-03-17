#include "map.h"
#include <SFML/Graphics.hpp>

Map::Map(World* world)
	: world(world)
{}

unsigned int Map::Width()
{
	return map_width;
}
unsigned int Map::Height()
{
	return map_height;
}

RGBColor Map::ObtainEntityColor(pool<Entity>::const_iterator entity)
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
		if (entity->Energy() < (entity->MaxEnergy() / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(255 * (entity->Energy() / (double)(entity->MaxEnergy() / 2))),
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(255 - 255 * (entity->Energy() / (double)entity->MaxEnergy())),
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
		if (entity->Hp() < (world->MaxHp() / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(255 * (entity->Hp() / (double)world->MaxHp())),
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(255 - 255 * (entity->Hp() / (double)world->MaxHp())),
				255, 
				0 
			};
		}
	}
	case view_settings::success:
	{
		switch (world->SuccessRule(entity))
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
			255 * ((double)entity->GetGenome().Generation()) / world->MaxGeneration()
		);
		return { c, c, c }; 
	}
	default:
		return {};
	}
}

RGBColor Map::ObtainColor(size_t x, size_t y)
{
	if (world->Terrain()[y][x].ContainsEntity() && view != view_settings::organic)
	{
		return ObtainEntityColor(world->Terrain()[y][x].GetEntity());
	}
	else if (world->Terrain()[y][x].IsContainsOrganic())
	{
		return	{ 0, 171, 209 };
	}

	return { 141, 219, 255 };
}

void Map::Draw(tgui::Canvas::Ptr canvas)
{
	canvas->clear({ 141, 219, 255 });

	sf::RectangleShape sprite;
	sprite.setOutlineThickness(0);
	sprite.setOutlineColor({ 0, 0, 0 });
	sf::Vector2f pos;
	sf::Vector2f size(cell_size, cell_size);
	sf::Vector2f osize(cell_outline, cell_outline);

	auto object = world->Organics().begin();
	while (object != world->Organics().end())
	{
		auto x = object->x();
		auto y = object->y();

		pos.x = x * cell_outline;
		pos.y = y * cell_outline;
		sprite.setPosition(pos);
		sprite.setSize(osize);

		sprite.setFillColor({ 0, 171, 209 });
		
		canvas->draw(sprite);

		object++;
	}

	if (view == view_settings::organic)
	{
		canvas->display();
		return;
	}

	auto entity = world->Entities().begin();
	sprite.setOutlineThickness(OUTLINE);
	while (entity != world->Entities().end())
	{
		auto x = entity->x();
		auto y = entity->y();

		pos.x = x * ((size_t)cell_size + OUTLINE);
		pos.y = y * ((size_t)cell_size + OUTLINE);
		sprite.setPosition(pos);
		sprite.setSize(size);

		auto color = ObtainEntityColor(entity);
		sprite.setFillColor(color);

		canvas->draw(sprite);

		entity++;
	}

	canvas->display();
}

size_t Map::GetMaxGeneration()
{
	return world->MaxGeneration();
}

void Map::ScaleCellSize(float scale)
{
	cell_size = cell_default_size * scale;
	cell_outline = cell_size + OUTLINE;
	map_width = world->Width() * cell_outline + 1;
	map_height = world->Height() * cell_outline + 1;
}

Info Map::GetInfoByCellsCoords(size_t x, size_t y)
{
	Info info;
	if (x >= world->Width() || y >= world->Height())
	{
		return info;
	}

	info.color = ObtainColor(x, y);

	info.light_power = (((world->Height() - (double)y) / world->Height()) * world->LightCoef()) * world->LightPower() * 2;
	info.contains_entity = world->Terrain()[y][x].ContainsEntity();
	if (world->Terrain()[y][x].ContainsEntity())
	{
		info.age.curr = world->Terrain()[y][x].GetEntity()->Age();
		info.age.max = world->Terrain()[y][x].GetEntity()->MaxAge();
		info.genome = world->Terrain()[y][x].GetEntity()->GetGenome().Data();
		info.genome_args = world->Terrain()[y][x].GetEntity()->GetGenome().Args();
		info.generation = world->Terrain()[y][x].GetEntity()->GetGenome().Generation();
		info.hp = world->Terrain()[y][x].GetEntity()->Hp();
		info.ch_of_mut = world->Terrain()[y][x].GetEntity()->GetGenome().MutationChance();
		info.energy = world->Terrain()[y][x].GetEntity()->Energy();
		info.max_energy = world->Terrain()[y][x].GetEntity()->MaxEnergy();
		info.max_hp = world->MaxHp();
	}
	if (world->Terrain()[y][x].IsContainsOrganic())
	{
		info.food_power = world->Terrain()[y][x].GetOrganic()->Energy();
	}
	return info;
}
Info Map::GetInfoByPixelCoords(size_t x_px, size_t y_px)
{
	return GetInfoByCellsCoords(x_px / cell_outline, y_px / cell_outline);
}

size_t Map::GetEntitiesCount()
{
	return world->Entities().size();
}
view_settings Map::GetView()
{
	return view;
}
void Map::SetView(view_settings new_val)
{
	view = new_val;
}