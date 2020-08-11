#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-system-d.lib")

#include <SFML/Graphics.hpp>

#include "environment.h"

using namespace sf;

int main()
{
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(800, 500), "Ecosystem");
	sf::RectangleShape background(sf::Vector2f(120.f, 50.f));
	background.setPosition(10.f, 50.f);
	background.setFillColor(sf::Color(150, 199, 255));
	background.setSize(sf::Vector2f(100.f, 100.f));
	
	Environment environment(new Cell(Gen({0,1,2,5,3,5,18,6})));

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		environment.Update();

		// Обрабатываем очередь событий в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				// Тогда закрываем его
				window.close();
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		window.draw(background);

		// Отрисовка окна	
		window.display();
	}

	return 0;
}
