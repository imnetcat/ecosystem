#include "map.h"
#include "colors.h"
using namespace Ecosystem::UI;
using namespace Ecosystem::Logic;

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

Color Map::ObtainEntityColor(pool<Entity>::const_iterator entity) const
{
	switch (mode)
	{
	case Mode::terrain:
		return ENTITY_COLOR;
	case Mode::organic:
		return ORGANIC_COLOR;
	case Mode::ration:
		return entity->GetGenome().Ration();
	case Mode::species:
		return entity->GetGenome().Species();
	case Mode::energy:
	{
		if (entity->Energy() < (entity->MaxEnergy() / 2))
		{
			return { 
				255, 
				static_cast<unsigned char>(
					255 * (entity->Energy() / (double)(entity->MaxEnergy() / 2))
				),
				0 
			};
		}
		else
		{
			return { 
				static_cast<unsigned char>(
					255 - 255 * (entity->Energy() / (double)entity->MaxEnergy())
				),
				255,
				0 
			};
		}
	}
	case Mode::age:
	{
		unsigned char c = static_cast<unsigned char>(
			255 - 255 * ((double)entity->Age() / entity->MaxAge())
		);
		return { c, c, c };
	}
	case Mode::success:
	{
		switch (world->SuccessRule(entity))
		{
		case Coefficient::reduce:
			return RED;
		case Coefficient::unchanged:
			return YELLOW;
		case Coefficient::enlarge:
			return GREEN;
		default:
			return BLACK;
		}
	}
	case Mode::generations:
	{
		unsigned char c = static_cast<unsigned char>(
			255 * ((double)entity->GetGenome().Generation()) / world->MaxGeneration()
		);
		return { c, c, c }; 
	}
	default:
		return BLACK;
	}
}

Color Map::ObtainColor(const cell* cell) const
{
	if (cell->ContainsEntity())
	{
		return ObtainEntityColor(cell->GetEntity());
	}
	else if (cell->IsContainsOrganic())
	{
		return	ORGANIC_COLOR;
	}

	return BLUE;
}

void Map::Draw(tgui::Canvas::Ptr canvas)
{
	canvas->clear(BLUE);

	sf::RectangleShape sprite;
	sprite.setOutlineThickness(0);
	sprite.setOutlineColor({ 0, 0, 0 });
	sf::Vector2f pos;
	sf::Vector2f size(cell_size, cell_size);
	sf::Vector2f osize(cell_outline, cell_outline);

	auto organic = world->Organics().begin();
	while (organic != world->Organics().end())
	{
		auto x = organic->x();
		auto y = organic->y();

		pos.x = x * cell_outline;
		pos.y = y * cell_outline;
		sprite.setPosition(pos);
		sprite.setSize(osize);

		sprite.setFillColor(ORGANIC_COLOR);
		
		canvas->draw(sprite);

		organic++;
	}

	if (mode == Mode::organic)
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

const cell* Map::GetCell(size_t x_px, size_t y_px)
{
	size_t x = x_px / cell_outline;
	size_t y = y_px / cell_outline;

	if (x >= world->Width() || y >= world->Height())
	{
		return nullptr;
	}

	return &world->Terrain()[y][x];
}

size_t Map::GetEntitiesCount()
{
	return world->Entities().size();
}
Map::Mode Map::GetMode()
{
	return mode;
}
void Map::SetMode(Mode new_val)
{
	mode = new_val;
}