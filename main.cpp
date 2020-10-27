#include "sfml_button.h"
#include "sfml_box.h"
#include "environment.h"

#include <iostream>
#include <string>

using namespace std;
using namespace sf;

const int SPEED = 10;

const int WINDOW_POS_X = 500;
const int WINDOW_POS_Y = 100;
const char* FONT = "SourceCodePro-Black.ttf";

const int VIEW_RECT_POS_X = 5;
const int VIEW_RECT_POS_Y = 15;
const int VIEW_RECT_SIZE_X = 190;
const int VIEW_RECT_SIZE_Y = 250;

const int DEFAULT_BTN_POS_X = 15;
const int DEFAULT_BTN_POS_Y = 35;
const int DEFAULT_BTN_SIZE_X = 75;
const int DEFAULT_BTN_SIZE_Y = 30;

const int ENERGY_BTN_POS_X = 15;
const int ENERGY_BTN_POS_Y = 70;
const int ENERGY_BTN_SIZE_X = 75;
const int ENERGY_BTN_SIZE_Y = 30;

const int SPECIES_BTN_POS_X = 110;
const int SPECIES_BTN_POS_Y = 35;
const int SPECIES_BTN_SIZE_X = 75;
const int SPECIES_BTN_SIZE_Y = 30;

const int RATION_BTN_POS_X = 110;
const int RATION_BTN_POS_Y = 70;
const int RATION_BTN_SIZE_X = 75;
const int RATION_BTN_SIZE_Y = 30;

const int AGE_BTN_POS_X = 110;
const int AGE_BTN_POS_Y = 105;
const int AGE_BTN_SIZE_X = 75;
const int AGE_BTN_SIZE_Y = 30;

const int HP_BTN_POS_X = 110;
const int HP_BTN_POS_Y = 140;
const int HP_BTN_SIZE_X = 75;
const int HP_BTN_SIZE_Y = 30;

const int SURVIVAL_BTN_POS_X = 15;
const int SURVIVAL_BTN_POS_Y = 140;
const int SURVIVAL_BTN_SIZE_X = 75;
const int SURVIVAL_BTN_SIZE_Y = 30;

const int MINERALS_BTN_POS_X = 15;
const int MINERALS_BTN_POS_Y = 105;
const int MINERALS_BTN_SIZE_X = 75;
const int MINERALS_BTN_SIZE_Y = 30;

const int INFO_RECT_POS_X = 5;
const int INFO_RECT_POS_Y = VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 25;
const int INFO_RECT_SIZE_X = 190;
const int INFO_RECT_SIZE_Y = 250;

const int CLEAR_INFO_POS_X = 70;
const int CLEAR_INFO_POS_Y = INFO_RECT_POS_Y + INFO_RECT_SIZE_Y - 10;
const int CLEAR_INFO_SIZE_X = 50;
const int CLEAR_INFO_SIZE_Y = 20;

int main()
{
	srand(time(0) - rand());
	int loopback = 0;
	// Объект, который, собственно, является главным окном приложения
	RenderWindow window(VideoMode(ENVIRONMENT_SIZE_X * CELL_OUTLINE, ENVIRONMENT_SIZE_Y * CELL_OUTLINE), "Ecosystem | Map");
	window.setPosition(sf::Vector2i(WINDOW_POS_X, WINDOW_POS_Y));

	RenderWindow setting_window(VideoMode(200, ENVIRONMENT_SIZE_Y * CELL_OUTLINE), "Ecosystem | Settings", sf::Style::Titlebar);
	setting_window.setPosition(sf::Vector2i(WINDOW_POS_X + ENVIRONMENT_SIZE_X * CELL_OUTLINE + 15, WINDOW_POS_Y));
	
	sf::RectangleShape setting_background(sf::Vector2f(setting_window.getSize().x, setting_window.getSize().y));
	setting_background.setFillColor({ 230, 230, 230 });

	sf::Font font;
	if (!font.loadFromFile(FONT))
	{
		cerr << "font doesn't loaded properly..." << endl;
	}

	Box view_rect(font, "view",
		VIEW_RECT_POS_X, 
		VIEW_RECT_POS_Y,
		VIEW_RECT_SIZE_X,
		VIEW_RECT_SIZE_Y, 16);
	
	Button ter_view_btn(font, "terrain", 
		DEFAULT_BTN_POS_X, 
		DEFAULT_BTN_POS_Y, 
		DEFAULT_BTN_SIZE_X, 
		DEFAULT_BTN_SIZE_Y, 14);

	Button energy_view_btn(font, "energy",
		ENERGY_BTN_POS_X,
		ENERGY_BTN_POS_Y,
		ENERGY_BTN_SIZE_X,
		ENERGY_BTN_SIZE_Y, 14);

	Button minerals_view_btn(font, "minerals",
		MINERALS_BTN_POS_X,
		MINERALS_BTN_POS_Y,
		MINERALS_BTN_SIZE_X,
		MINERALS_BTN_SIZE_Y, 14);

	Button species_view_btn(font, "species",
		SPECIES_BTN_POS_X,
		SPECIES_BTN_POS_Y,
		SPECIES_BTN_SIZE_X,
		SPECIES_BTN_SIZE_Y, 14);

	Button age_view_btn(font, "age",
		AGE_BTN_POS_X,
		AGE_BTN_POS_Y,
		AGE_BTN_SIZE_X,
		AGE_BTN_SIZE_Y, 14);

	Button ration_view_btn(font, "ration",
		RATION_BTN_POS_X,
		RATION_BTN_POS_Y,
		RATION_BTN_SIZE_X,
		RATION_BTN_SIZE_Y, 14);

	Button hp_view_btn(font, "hp",
		HP_BTN_POS_X,
		HP_BTN_POS_Y,
		HP_BTN_SIZE_X,
		HP_BTN_SIZE_Y, 14);

	Button survival_view_btn(font, "survival",
		SURVIVAL_BTN_POS_X,
		SURVIVAL_BTN_POS_Y,
		SURVIVAL_BTN_SIZE_X,
		SURVIVAL_BTN_SIZE_Y, 14);

	Box info_rect(font, "info", INFO_RECT_POS_X, INFO_RECT_POS_Y, INFO_RECT_SIZE_X, INFO_RECT_SIZE_Y, 16);
	Box cell_ico(font, "", 10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 30, 50, 50, 16);
	
	sf::Text field_title;
	field_title.setFont(font);
	field_title.setCharacterSize(12);
	field_title.setPosition(85, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 45);
	field_title.setFillColor(sf::Color::Black);
	
	sf::Text cell_generation;
	cell_generation.setFont(font);
	cell_generation.setCharacterSize(12);
	cell_generation.setPosition(65, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 60);
	cell_generation.setFillColor(sf::Color::Black);

	sf::Text cell_age;
	cell_age.setFont(font);
	cell_age.setCharacterSize(12);
	cell_age.setPosition(65, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 75);
	cell_age.setFillColor(sf::Color::Black);

	sf::Text cell_mutant_chance;
	cell_mutant_chance.setFont(font);
	cell_mutant_chance.setCharacterSize(12);
	cell_mutant_chance.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 90);
	cell_mutant_chance.setFillColor(sf::Color::Black);

	sf::Text cell_energy;
	cell_energy.setFont(font);
	cell_energy.setCharacterSize(12);
	cell_energy.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 105);
	cell_energy.setFillColor(sf::Color::Black);

	sf::Text cell_hp;
	cell_hp.setFont(font);
	cell_hp.setCharacterSize(12);
	cell_hp.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 120);
	cell_hp.setFillColor(sf::Color::Black);

	sf::Text field_food;
	field_food.setFont(font);
	field_food.setCharacterSize(12);
	field_food.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 135);
	field_food.setFillColor(sf::Color::Black);

	sf::Text field_light;
	field_light.setFont(font);
	field_light.setCharacterSize(12);
	field_light.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 150);
	field_light.setFillColor(sf::Color::Black);

	sf::Text cell_genom;
	cell_genom.setFont(font);
	cell_genom.setCharacterSize(12);
	cell_genom.setPosition(10, VIEW_RECT_POS_Y + VIEW_RECT_SIZE_Y + 165);
	cell_genom.setFillColor(sf::Color::Black);

	Button info_clear_rect_btn(font, "clear",
		CLEAR_INFO_POS_X,
		CLEAR_INFO_POS_Y,
		CLEAR_INFO_SIZE_X,
		CLEAR_INFO_SIZE_Y, 12);
	
	ter_view_btn.Disable(true);

	sf::RectangleShape sprite_cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	sf::RectangleShape sprite_struct(sf::Vector2f(CELL_OUTLINE, CELL_OUTLINE));

	Environment environment;

	bool turn_on_info_block = false;
	bool pause = false;

	// Главный цикл приложения. Выполняется, пока открыто окно
	while (window.isOpen())
	{
		setting_window.clear();

		// Обрабатываем очередь событий в цикле
		Event main_event, settings_event;
		while (setting_window.pollEvent(settings_event))
		{
			if (settings_event.type == sf::Event::MouseButtonPressed)
			{
				// Пользователь нажал и отпустил лкм
				if (settings_event.mouseButton.button == sf::Mouse::Left)
				{
					// нажатие на кнопку выключения окна информации
					if (info_clear_rect_btn.IsClicked(settings_event.mouseButton.x, 
						settings_event.mouseButton.y))
					{
						turn_on_info_block = false;
					}

					if (age_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(true);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(age);
					}

					if (ter_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(true);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						age_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(terrain);
					}

					if (species_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(true);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(species);
					}

					if (energy_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(true);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(energy);
					}

					if (minerals_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(true);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(view_settings::minerals);
					}

					if (ration_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(true);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(false);
						environment.SetView(view_settings::ration);
					}

					if (hp_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(true);
						survival_view_btn.Disable(false);
						environment.SetView(view_settings::hp);
					}

					if (survival_view_btn.IsClicked(settings_event.mouseButton.x,
						settings_event.mouseButton.y))
					{
						ter_view_btn.Disable(false);
						species_view_btn.Disable(false);
						energy_view_btn.Disable(false);
						minerals_view_btn.Disable(false);
						ration_view_btn.Disable(false);
						age_view_btn.Disable(false);
						hp_view_btn.Disable(false);
						survival_view_btn.Disable(true);
						environment.SetView(view_settings::survival);
					}
				}
			}
		}
		while (window.pollEvent(main_event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (main_event.type == Event::Closed)
			{
				// Тогда закрываем его
				setting_window.close();
				window.close();
			}
			
			// Пользователь нажал и отпустил какую-то из кнопок
			if (main_event.type == sf::Event::KeyReleased)
			{
				// turn on\off pause
				if (main_event.key.code == sf::Keyboard::P)
				{
					pause = !pause;
				}
			}

			if (main_event.type == sf::Event::MouseButtonPressed)
			{
				// Пользователь нажал и отпустил лкм
				if (main_event.mouseButton.button == sf::Mouse::Left)
				{
					auto info = environment.GetInfo(main_event.mouseButton.x, main_event.mouseButton.y);
					turn_on_info_block = true;
					cell_ico.Color({ info.color.r, info.color.g, info.color.b });
					field_title.setString("Structure");
					string genom = "\n";
					if (info.genom.size())
					{
						field_title.setString("Cell");
						size_t ndl = 0;
						for (auto command : info.genom)
						{
							genom += to_string(static_cast<unsigned int>(command)) + ' ';
							ndl++;
							if (ndl == 11)
							{
								ndl = 0;
								genom += '\n';
							}
						}
						cell_genom.setString("Genom: " + genom);
						cell_generation.setString("Generation: " + to_string(info.generation));
						cell_age.setString("Age: " + to_string(info.age.curr) + "/" + to_string(info.age.max));
						cell_mutant_chance.setString("Chance of mutation: " + to_string(info.ch_of_mut).substr(0, 4));
						cell_energy.setString("Energy: " + to_string(info.energy) + "/" + to_string(MAX_ENERGY));
						cell_hp.setString("Hp: " + to_string(info.hp) + "/" + to_string(MAX_HP));
					}
					else
					{
						cell_genom.setString("Genom: -");
						cell_generation.setString("Generation: -");
						cell_age.setString("Age: -");
						cell_mutant_chance.setString("Chance of mutation: -");
						cell_energy.setString("Energy: -");
						cell_hp.setString("Hp: -");
					}
					field_food.setString("Food  power: " + to_string(info.food_power));
					field_light.setString("Light power: " + to_string(info.light_power));
				}
			}
		}

		setting_window.draw(setting_background);

		view_rect.Draw(setting_window);
		ter_view_btn.Draw(setting_window);
		energy_view_btn.Draw(setting_window);
		minerals_view_btn.Draw(setting_window);
		species_view_btn.Draw(setting_window);
		age_view_btn.Draw(setting_window);
		ration_view_btn.Draw(setting_window);
		info_rect.Draw(setting_window);
		hp_view_btn.Draw(setting_window);
		survival_view_btn.Draw(setting_window);

		if (turn_on_info_block)
		{
			cell_ico.Draw(setting_window);

			setting_window.draw(field_title);
			setting_window.draw(cell_genom);
			setting_window.draw(cell_generation);
			setting_window.draw(cell_age);
			setting_window.draw(field_food);
			setting_window.draw(field_light);
			setting_window.draw(cell_mutant_chance);
			setting_window.draw(cell_energy);
			setting_window.draw(cell_hp);
			info_clear_rect_btn.Draw(setting_window);
		}


		// clear the window and draw background with background color
		window.clear(sf::Color(0, 61, 156));
		// update environment
		//if(!loopback)
		if (!loopback && !pause)
		{
			environment.Update();
		}

		auto envdata = environment.UpdateColors();
		// draw all entities
		for (const auto& object : envdata)
		{
			if (object.outline)
			{
				sprite_cell.setPosition(object.position.x, object.position.y);
				sprite_cell.setOutlineThickness(OUTLINE);
				sprite_cell.setFillColor({ object.color.r, object.color.g, object.color.b,
					(unsigned char)((unsigned char)255 - object.shadow) });
				sprite_cell.setOutlineColor(sf::Color(0, 0, 0,
					(unsigned char)((unsigned char)255 - object.shadow)));
				window.draw(sprite_cell);
			}
			else
			{
				sprite_struct.setPosition(object.position.x, object.position.y);
				sprite_struct.setFillColor({ object.color.r, object.color.g, object.color.b,
					(unsigned char)((unsigned char)255 - object.shadow) });
				sprite_struct.setOutlineColor(sf::Color(object.color.r, object.color.g, object.color.b,
					(unsigned char)((unsigned char)255 - object.shadow)));
				window.draw(sprite_struct);
			}
		}

		// Отрисовка окна
		window.display();

		loopback++;
		
		if (loopback == SPEED)
			loopback = 0;

		setting_window.display();
	}

	return 0;
}
