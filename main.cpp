#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-system-d.lib")

#include <SFML/Graphics.hpp>

#include "environment.h"

#include <iostream>

using namespace std;
using namespace sf;

const int SPEED = 1;

int main()
{
	int loopback = 0;
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(ENVIRONMENT_SIZE_X * CELL_OUTLINE, ENVIRONMENT_SIZE_Y * CELL_OUTLINE), "Ecosystem");

	Environment environment(
		ENVIRONMENT_SIZE_X / 2, 
		ENVIRONMENT_SIZE_Y / 2,
		shared_ptr<Cell>(new Cell(Ration{ 2,1,2 }, Gen({
			move_left,
			fotosintesis,
			fotosintesis,
			move_bottom,
			fotosintesis,
			move_right,
			furcation,
			fotosintesis,
			fotosintesis,
			move_top,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			furcation,
			fotosintesis,
		}, 100)))
	);

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				// Тогда закрываем его
				window.close();
		}

		if (!loopback)
		{
			// clear the window and draw background with background color
			window.clear(sf::Color(0, 61, 156));

			/*
			// set up background light levels
			sf::RectangleShape background_light_level(sf::Vector2f(ENVIRONMENT_SIZE_X, Environment::light_level::surface));
			background_light_level.setPosition(0, 0);
			background_light_level.setFillColor({ 176, 207, 255 });
			window.draw(background_light_level);

			background_light_level = sf::RectangleShape(sf::Vector2f(ENVIRONMENT_SIZE_X, Environment::light_level::depth));
			background_light_level.setPosition(0, CELL_SIZE * 4);
			background_light_level.setFillColor({ 92, 156, 255 });
			window.draw(background_light_level);

			background_light_level = sf::RectangleShape((sf::Vector2f(ENVIRONMENT_SIZE_X, Environment::light_level::shallow)));
			background_light_level.setPosition(0, CELL_SIZE * 4 + CELL_SIZE * 8);
			background_light_level.setFillColor({ 0, 100, 255 });
			window.draw(background_light_level);
			*/
			// update environment
			auto envdata = environment.Update();
			// draw all entities
			for (const auto& entity : envdata)
			{
				sf::RectangleShape entity_sprite(sf::Vector2f(CELL_SIZE, CELL_SIZE));
				entity_sprite.setPosition(entity.position.x, entity.position.y);
				entity_sprite.setFillColor({ entity.color.r, entity.color.g, entity.color.b });
				entity_sprite.setOutlineThickness(OUTLINE);
				if (entity.outline)
				{
					entity_sprite.setOutlineColor(sf::Color(0,0,0));
				}
				else
				{
					entity_sprite.setOutlineColor(sf::Color(entity.color.r, entity.color.g, entity.color.b));
				}

				window.draw(entity_sprite);
			}

			// Отрисовка окна	
			window.display();
		}

		loopback++;
		if (loopback == 100000 / SPEED)
			loopback = 0;

	}

	return 0;
}
