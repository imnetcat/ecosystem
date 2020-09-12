#pragma once
#include "sfml.h"
#include <string>

class Button
{
private:
	const int pos_x;
	const int pos_y;
	const int size_x;
	const int size_y;

	sf::RectangleShape rect;
	sf::Text title;
public:
	Button(sf::Font& font, const std::string& title_text, int posx, int posy, int sizex, int sizey, int font_size = 16)
		: pos_x(posx), pos_y(posy), size_x(sizex), size_y(sizey)
	{
		rect = sf::RectangleShape(sf::Vector2f(size_x, size_y));
		rect.setPosition(pos_x, pos_y);
		rect.setOutlineThickness(1);
		rect.setFillColor({ 255, 255, 255 });
		rect.setOutlineColor(sf::Color(0, 0, 0));

		title.setFont(font);
		title.setString(title_text);
		title.setCharacterSize(font_size);
		title.setFillColor(sf::Color::Black);

		sf::FloatRect textRect = title.getLocalBounds();
		title.setOrigin(textRect.left + textRect.width / 2.0f,
			textRect.top + textRect.height / 2.0f);

		title.setPosition(posx + sizex / 2, posy + sizey / 2);
	}

	void Disable(bool flag)
	{
		if(flag)
			rect.setFillColor({ 230, 230, 230 });
		else
			rect.setFillColor({ 255, 255, 255 });
	}

	void Draw(sf::RenderWindow& window) const
	{
		window.draw(rect);
		window.draw(title);
	}

	bool IsClicked(int x, int y) const
	{
		return x > pos_x &&
			x < pos_x + size_x &&
			y > pos_y &&
			y < pos_y + size_y;
	}
};
