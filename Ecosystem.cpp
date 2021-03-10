#include "Ecosystem.h"
#include "Environment.h"
#include "config.h"
#include <SFML/Graphics.hpp>

Ecosystem::Ecosystem()
	: view(view_settings::terrain) {}

RGBColor Ecosystem::ObtainEntityColor(EntitiesIterator entity)
{
	switch (view)
	{
	case view_settings::terrain:
		return { 13, 168, 19 };
	case view_settings::organic:
		return { 209, 209, 209 };
	case view_settings::ration:
	{
		Ration ration = entity->GetGenome().Ration();

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
		if (entity->Energy() < (MAX_ENERGY / 2))
		{
			return { 255, static_cast<unsigned char>(255 * (entity->Energy() / (double)(MAX_ENERGY / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(255 - 255 * (entity->Energy() / (double)MAX_ENERGY)), 255, 0 };
		}
	}
	case view_settings::species:
		return entity->GetGenome().Species();
	case view_settings::age:
	{
		unsigned char c = static_cast<unsigned char>(255 - 255 * ((double)entity->Age() / entity->MaxAge()));
		return { c, c, c };
	}
	case view_settings::hp:
	{
		if (entity->Hp() < (MAX_HP / 2))
		{
			return { 191, static_cast<unsigned char>(191 * (entity->Hp() / (double)(MAX_HP / 2))), 0 };
		}
		else
		{
			return { static_cast<unsigned char>(191 * ((double)(MAX_HP / 2) / entity->Hp())), 191, 0 };
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
		unsigned char c = static_cast<unsigned char>(255 * ((double)entity->GetGenome().Generation()) / max_generation);
		return { c, c, c }; }
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
	sf::Vector2f size(CELL_SIZE, CELL_SIZE);
	sf::Vector2f osize(CELL_OUTLINE, CELL_OUTLINE);

	auto object = organic.begin();
	while (object != organic.end())
	{
		auto x = object->GetX();
		auto y = object->GetY();

		pos.x = x * CELL_OUTLINE;
		pos.y = y * CELL_OUTLINE;
		sprite.setPosition(pos);
		sprite.setSize(osize);
		sprite.setFillColor({ 0, 171, 209 });
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

		pos.x = x * (CELL_SIZE + OUTLINE);
		pos.y = y * (CELL_SIZE + OUTLINE);
		sprite.setPosition(pos);
		sprite.setSize(size);

		sprite.setFillColor(ObtainEntityColor(entity));

		canvas->draw(sprite);

		entity++;
	}

	canvas->display();
}

Info Ecosystem::GetInfo(size_t x_px, size_t y_px)
{
	size_t x = x_px / CELL_OUTLINE;
	size_t y = y_px / CELL_OUTLINE;
	Info info;
	cout << x << " " << y << endl;

	info.color = ObtainColor(x, y);

	info.light_power = (((ENVIRONMENT_SIZE_Y - (double)y) / ENVIRONMENT_SIZE_Y) * LIGHT_COEF) * LIGHT_POWER;
	info.contains_entity = terrain[y][x].ContainsEntity();
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
	if (terrain[y][x].IsContainsOrganic())
	{
		info.food_power = terrain[y][x].GetOrganic()->Energy();
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