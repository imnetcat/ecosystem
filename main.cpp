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

	Environment environment;

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

			// update environment
			auto envdata = environment.Update();
			// draw all entities
			for (const auto& object : envdata)
			{
				sf::RectangleShape sprite(sf::Vector2f(CELL_SIZE, CELL_SIZE));

				// object
				sprite.setPosition(object.position.x, object.position.y);

				sprite.setFillColor({ object.color.r, object.color.g, object.color.b, 
					(unsigned char)((unsigned char)255 - object.shadow) });
				
				sprite.setOutlineThickness(OUTLINE);
				if (object.outline)
				{
					sprite.setOutlineColor(sf::Color(0, 0, 0));
				}
				else
				{
					sprite.setOutlineColor(sf::Color(object.color.r, object.color.g, object.color.b, (unsigned char)((unsigned char)255 - object.shadow)));
				}
				

				window.draw(sprite);
			}

			// Отрисовка окна	
			window.display();
		}

		loopback++;
		if (loopback == 1 / SPEED)
			loopback = 0;

	}

	return 0;
}
