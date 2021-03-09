#include "Ecosystem.h"
#include "config.h"

#include <chrono> 
using namespace std::chrono;

#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 600;
const int MENUBAR_HEIGHT = 22;
const int SIDEBAR_WIDTH = WINDOW_WIDTH * 0.2;
const int MAP_WIDTH = ENVIRONMENT_SIZE_X * CELL_OUTLINE + 1;
const int MAP_HEIGHT = ENVIRONMENT_SIZE_Y * CELL_OUTLINE + 1;

void DisableViewMenuItem(tgui::MenuBar::Ptr menu, const tgui::String& item)
{
	menu->setMenuItemEnabled({ 
		 "View", "Terrain"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Energy"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Organic"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Species"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Age"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Ration"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Hp"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Success"
	}, true);
	menu->setMenuItemEnabled({
		 "View", "Generation"
	}, true);

	menu->setMenuItemEnabled({ "View", item }, false);
}

int main()
{
	bool turn_on_info_block = false;
	bool pause = false;
	Ecosystem ecosystem;
	Ecosystem* ecosys_ptr = &ecosystem;

	sf::Clock Framerate;
	float acu = 0;
	float frametime = 0.03;

	sf::RenderWindow main({ WINDOW_WIDTH, WINDOW_HEIGHT }, "Ecosystem");
	main.setPosition(sf::Vector2i(350, 250));

	tgui::GuiSFML gui(main);
	tgui::GuiSFML* gui_ptr = &gui;

	tgui::MenuBar::Ptr menu = tgui::MenuBar::create();
	menu->setHeight(MENUBAR_HEIGHT);
	menu->addMenu("Environment");
	menu->addMenuItem("Pause");
	menu->addMenu("View");
	menu->addMenuItem("Terrain");
	menu->addMenuItem("Energy");
	menu->addMenuItem("Organic");
	menu->addMenuItem("Species");
	menu->addMenuItem("Age");
	menu->addMenuItem("Ration");
	menu->addMenuItem("Hp");
	menu->addMenuItem("Success");
	menu->addMenuItem("Generation");
	menu->setMenuItemEnabled({ "View", "Terrain" }, false);

	menu->connectMenuItem({ "Environment", "Pause" }, [&pause]() {
		pause = !pause;
	});

	menu->connectMenuItem({ "View", "Terrain" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Terrain");
		ecosys_ptr->SetView(view_settings::terrain);
	});
	menu->connectMenuItem({ "View", "Energy" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Energy");
		ecosys_ptr->SetView(view_settings::energy);
	});
	menu->connectMenuItem({ "View", "Organic" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Organic");
		ecosys_ptr->SetView(view_settings::organic);
	});
	menu->connectMenuItem({ "View", "Species" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Species");
		ecosys_ptr->SetView(view_settings::species);
	});
	menu->connectMenuItem({ "View", "Age" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Age");
		ecosys_ptr->SetView(view_settings::age);
	});
	menu->connectMenuItem({ "View", "Ration" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Ration");
		ecosys_ptr->SetView(view_settings::ration);
	});
	menu->connectMenuItem({ "View", "Hp" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Hp");
		ecosys_ptr->SetView(view_settings::hp);
	});
	menu->connectMenuItem({ "View", "Success" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Success");
		ecosys_ptr->SetView(view_settings::success);
	});
	menu->connectMenuItem({ "View", "Generation" }, [menu, ecosys_ptr]() {
		DisableViewMenuItem(menu, "Generation");
		ecosys_ptr->SetView(view_settings::generations);
	});
	gui.add(menu);

	auto sidebar_layout = tgui::VerticalLayout::create();
	auto sidebar = tgui::Panel::create();
	sidebar->setSize("100%", "100%");
	sidebar_layout->setPosition(0, MENUBAR_HEIGHT + 1);
	sidebar_layout->setSize(SIDEBAR_WIDTH - 1, "100%");
	sidebar_layout->add(sidebar);
	gui.add(sidebar_layout);

	auto map_layout = tgui::VerticalLayout::create();
	auto map = tgui::ScrollablePanel::create();
	auto canvas = tgui::Canvas::create();
	map_layout->setSize(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT - MENUBAR_HEIGHT);
	map_layout->setPosition(SIDEBAR_WIDTH, MENUBAR_HEIGHT + 1);
	map->setSize(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT - MENUBAR_HEIGHT);
	canvas->setSize(MAP_WIDTH, MAP_HEIGHT);
	map->add(canvas);
	map_layout->add(map);
	gui.add(map_layout);

	sf::VertexArray cell_image(sf::Quads, 4);
	cell_image[0].position = sf::Vector2f(1, 1);
	cell_image[1].position = sf::Vector2f(51, 1);
	cell_image[2].position = sf::Vector2f(51, 51);
	cell_image[3].position = sf::Vector2f(1, 51);

	auto cell_image_canvas = tgui::Canvas::create({ 200, 140 });
	cell_image_canvas->setSize(52, 52);
	cell_image_canvas->setPosition(10, 10);

	cell_image_canvas->clear();
	cell_image_canvas->draw(cell_image);
	cell_image_canvas->display();
	sidebar->add(cell_image_canvas);

	auto info_title = tgui::Label::create();
	info_title->setPosition(125, 10);
	info_title->setSize(100, 18);
	info_title->setTextSize(13);
	info_title->setText("cell");
	sidebar->add(info_title);

	auto label = tgui::Label::create();
	label->setPosition(72, 28);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("age:");
	sidebar->add(label);
	auto info_age = tgui::Label::create();
	info_age->setPosition(110, 28);
	info_age->setSize(100, 18);
	info_age->setTextSize(13);
	sidebar->add(info_age);

	label = tgui::Label::create();
	label->setPosition(72, 46);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("hp:");
	sidebar->add(label);
	auto info_hp = tgui::Label::create();
	info_hp->setPosition(110, 46);
	info_hp->setSize(100, 18);
	info_hp->setTextSize(13);
	sidebar->add(info_hp);

	label = tgui::Label::create();
	label->setPosition(10, 75);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("generation:");
	sidebar->add(label);
	auto info_generation = tgui::Label::create();
	info_generation->setPosition(110, 75);
	info_generation->setSize(100, 18);
	info_generation->setTextSize(13);
	sidebar->add(info_generation);

	label = tgui::Label::create();
	label->setPosition(10, 100);
	label->setSize(125, 18);
	label->setTextSize(13);
	label->setText("mutation chance:");
	sidebar->add(label);
	auto info_mutation_chance = tgui::Label::create();
	info_mutation_chance->setPosition(125, 100);
	info_mutation_chance->setSize(100, 18);
	info_mutation_chance->setTextSize(13);
	sidebar->add(info_mutation_chance);

	label = tgui::Label::create();
	label->setPosition(10, 125);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("energy:");
	sidebar->add(label);
	auto info_energy = tgui::Label::create();
	info_energy->setPosition(110, 125);
	info_energy->setSize(100, 18);
	info_energy->setTextSize(13);
	sidebar->add(info_energy);

	label = tgui::Label::create();
	label->setPosition(10, 150);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("organic power:");
	sidebar->add(label);
	auto info_organic_power = tgui::Label::create();
	info_organic_power->setPosition(110, 150);
	info_organic_power->setSize(100, 18);
	info_organic_power->setTextSize(13);
	sidebar->add(info_organic_power);

	label = tgui::Label::create();
	label->setPosition(10, 175);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("light power:");
	sidebar->add(label);
	auto info_light_power = tgui::Label::create();
	info_light_power->setPosition(110, 175);
	info_light_power->setSize(100, 18);
	info_light_power->setTextSize(13);
	sidebar->add(info_light_power);

	label = tgui::Label::create();
	label->setPosition(10, 200);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("genome:");
	sidebar->add(label);
	auto info_genome = tgui::Label::create();
	info_genome->setPosition(110, 200);
	info_genome->setSize(100, 18);
	info_genome->setTextSize(13);
	sidebar->add(info_genome);

	auto SetInfoBox = [
		&turn_on_info_block, ecosys_ptr, info_genome, info_light_power,
		info_organic_power, info_mutation_chance,
		info_energy, info_generation,
		info_hp, info_age, info_title,
		&cell_image, cell_image_canvas](const sf::Vector2f& mousePos)
	{
		auto info = ecosys_ptr->GetInfo(mousePos.x, mousePos.y);
		turn_on_info_block = true;
		Color color(info.color.r, info.color.g, info.color.b);
		cell_image[0].color = color;
		cell_image[1].color = color;
		cell_image[2].color = color;
		cell_image[3].color = color;
		if (info.contains_entity)
		{
			info_title->setText("Entity");
			info_genome->setText(to_string(info.genome));
			info_generation->setText(to_string(info.generation));
			info_age->setText(to_string(info.age.curr) + "/" + to_string(info.age.max));
			info_mutation_chance->setText(to_string(info.ch_of_mut));
			info_energy->setText(to_string(info.energy) + "/" + to_string(MAX_ENERGY));
			info_hp->setText(to_string(info.hp) + "/" + to_string(MAX_HP));
		}
		else
		{
			info_title->setText("Cell");
			info_genome->setText("-");
			info_generation->setText("-");
			info_age->setText("-");
			info_mutation_chance->setText("-");
			info_energy->setText("-");
			info_hp->setText("-");
		}
		info_organic_power->setText(to_string(info.food_power));
		info_light_power->setText(to_string(info.light_power));

		cell_image_canvas->clear();
		cell_image_canvas->draw(cell_image);
		cell_image_canvas->display();
	};

	canvas->onClick(SetInfoBox);

	auto ClearInfoBox = [
		info_genome, info_light_power, 
		info_organic_power, info_mutation_chance, 
		info_energy, info_generation, 
		info_hp, info_age, info_title,
		&cell_image, cell_image_canvas ]()
	{
		cell_image[0].color = sf::Color::White;
		cell_image[1].color = sf::Color::White;
		cell_image[2].color = sf::Color::White;
		cell_image[3].color = sf::Color::White;

		cell_image_canvas->clear();
		cell_image_canvas->draw(cell_image);
		cell_image_canvas->display();

		info_title->setText("Cell");
		info_genome->setText("-");
		info_generation->setText("-");
		info_age->setText("-");
		info_mutation_chance->setText("-");
		info_energy->setText("-");
		info_hp->setText("-");
		info_organic_power->setText("-");
		info_light_power->setText("-");
	};

	ClearInfoBox();

	tgui::Button::Ptr info_clear_rect_btn = tgui::Button::create();
	info_clear_rect_btn->setPosition(80, 228);
	info_clear_rect_btn->setText("Clear");
	info_clear_rect_btn->onPress(ClearInfoBox);
	sidebar->add(info_clear_rect_btn, "info_clear");

	while (main.isOpen())
	{
		acu += Framerate.restart().asSeconds();

		Event event;
		while (main.pollEvent(event))
		{
			gui.handleEvent(event);

			if (event.type == sf::Event::Closed)
				main.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					pause = !pause;
				}
			}
		}

		// logic
		if (acu > frametime && !pause)
		{
			// clear the window and draw background with background color
			// update ecosystem and draw sprites

			//auto start = high_resolution_clock::now();
			ecosystem.Update();
			//auto stop = high_resolution_clock::now();
			//auto duration = duration_cast<std::chrono::microseconds>(stop - start);
			//cout << "Environment update: \t" << duration.count() << " ms" << endl;
		}

		// Drawing
		main.clear();

		ecosystem.Draw(canvas);

		gui.draw();
		main.display();
	}

	return 0;
}

