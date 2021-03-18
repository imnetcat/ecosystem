#include "ui/gui.h"
#include "ui/map.h"
#include <chrono> 
using namespace std::chrono;

#include <iostream>
#include <string>
#include <map>

#include <windows.h>
#include <ShellApi.h>

using namespace Ecosystem::Logic;
using namespace Ecosystem::UI;
using namespace std;
using namespace sf;

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 600;
const int SIDEBAR_WIDTH = WINDOW_WIDTH * 0.2;

int main()
{
	int SPEED = 0;

	bool do_tic = false;
	bool pause = false;
	bool hibernate = false;

	unsigned __int64 GENOME_SEED = random::Generate();
	unsigned int WORLD_WIDTH		= 100;
	unsigned int WORLD_HEIGHT		= 51;
	unsigned short  LIGHT_POWER		= 2000;
	unsigned short MAX_ORGANIC_TO_EAT = 3000;
	unsigned short MAX_ENTITIES_TO_EAT = 6000;
	unsigned short MAX_ENERGY = 20000;

	Genome::Init(GENOME_SEED);
	World world(
		WORLD_WIDTH, 
		WORLD_HEIGHT,
		LIGHT_POWER, 
		MAX_ORGANIC_TO_EAT, 
		MAX_ENTITIES_TO_EAT, 
		MAX_ENERGY
	);

	Map world_map(&world);

	Map* ecosys_ptr = &world_map;

	sf::RenderWindow main({ WINDOW_WIDTH, WINDOW_HEIGHT }, "Ecosystem", sf::Style::Titlebar | sf::Style::Close);
	main.setPosition(sf::Vector2i(350, 250));

	gui gui(main);
	tgui::GuiSFML* gui_ptr = &gui;

	gui.menubar({ "About", "Help" }, []() {
		ShellExecuteA(0, 0, "https://github.com/imnetcat/world_map/wiki", NULL, NULL, SW_SHOW);
	});
	gui.menubar({ "About", "Repository" }, []() {
		ShellExecuteA(0, 0, "https://github.com/imnetcat/world_map", NULL, NULL, SW_SHOW);
	});
	gui.menubar({ "About", "Creator" }, []() {
		ShellExecuteA(0, 0, "https://github.com/imnetcat", NULL, NULL, SW_SHOW);
	});

	gui.menubar({ "World", "Run" }, [&hibernate, &pause]() {
		if (hibernate)
		{
			return;
		}
		pause = false;
	});
	gui.menubar({ "World", "Pause" }, [&hibernate , &pause]() {
		if (hibernate)
		{
			return;
		}
		pause = true;
	});
	gui.menubar({ "World", "By tic" }, [&hibernate, &pause, &do_tic]() {
		if (hibernate)
		{
			return;
		}
		pause = true;
		do_tic = true;
	});

	gui.menubar({ "View", "Terrain" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::terrain);
	});
	gui.menubar({ "View", "Energy" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::energy);
	});
	gui.menubar({ "View", "Organic" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::organic);
	});
	gui.menubar({ "View", "Species" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::species);
	});
	gui.menubar({ "View", "Age" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::age);
	});
	gui.menubar({ "View", "Ration" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::ration);
	});
	gui.menubar({ "View", "Success" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::success);
	});
	gui.menubar({ "View", "Generation" }, [ecosys_ptr]() {
		ecosys_ptr->SetMode(Map::Mode::generations);
	});

	auto map_layout = tgui::VerticalLayout::create();
	auto map = tgui::ScrollablePanel::create();
	auto canvas = tgui::Canvas::create();
	map_layout->setSize(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT - gui.MENUBAR_HEIGHT);
	map_layout->setPosition(SIDEBAR_WIDTH, gui.MENUBAR_HEIGHT + 1);
	map->setSize(WINDOW_WIDTH - SIDEBAR_WIDTH, WINDOW_HEIGHT - gui.MENUBAR_HEIGHT);
	canvas->setSize(world_map.Width(), world_map.Height());
	map->add(canvas);
	map_layout->add(map);
	gui.add(map_layout);

	auto sidebar_layout = tgui::VerticalLayout::create();
	sidebar_layout->setPosition(0, gui.MENUBAR_HEIGHT + 1);
	sidebar_layout->setSize(SIDEBAR_WIDTH - 1, "100%");
	gui.add(sidebar_layout);

	auto stats_panel = tgui::Panel::create();
	stats_panel->setSize("100%", 100);
	sidebar_layout->add(stats_panel);

	auto info_panel = tgui::Panel::create();
	info_panel->setPosition(0, 110);
	info_panel->setSize("100%", WINDOW_HEIGHT - gui.MENUBAR_HEIGHT - 110);
	sidebar_layout->add(info_panel);

	auto label = tgui::Label::create();
	label->setPosition(10, 10);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("Zoom:");
	stats_panel->add(label);
	auto zoom_slider = tgui::Slider::create();
	zoom_slider->setPosition(10, 30);
	zoom_slider->setSize(200, 18);
	zoom_slider->setValue(1.0);
	zoom_slider->setMaximum(1.5);
	zoom_slider->setStep(0.1);
	zoom_slider->setMinimum(0.1);
	stats_panel->add(zoom_slider);
	label = tgui::Label::create();
	label->setPosition(10, 60);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("Speed:");
	stats_panel->add(label);
	auto speed_slider = tgui::Slider::create();
	speed_slider->setPosition(10, 80);
	speed_slider->setSize(200, 18);
	speed_slider->setValue(0);
	speed_slider->setMaximum(50);
	speed_slider->setStep(1);
	speed_slider->setMinimum(0);
	speed_slider->setInvertedDirection(true);
	stats_panel->add(speed_slider);

	gui.menubar({ "World", "Hybernate" }, [&pause, speed_slider, canvas, &hibernate]() {
		hibernate = !hibernate;
		pause = false;
		if (hibernate)
		{
			canvas->clear(sf::Color::White);
			canvas->display();
			speed_slider->setValue(0);
			speed_slider->setEnabled(false);
		}
		else
		{
			speed_slider->setEnabled(true);
		}
	});

	auto Zoom = [canvas, ecosys_ptr](float scale)
	{
		ecosys_ptr->ScaleCellSize(scale);
		canvas->setSize(ecosys_ptr->Width(), ecosys_ptr->Height());
	};
	auto Speed = [&SPEED](float coef)
	{
		SPEED = coef;
	};

	zoom_slider->onValueChange(Zoom);
	speed_slider->onValueChange(Speed);

	label = tgui::Label::create();
	label->setPosition(10, 110);
	label->setSize(110, 18);
	label->setTextSize(13);
	label->setText("Genome seed:");
	stats_panel->add(label);
	auto genome_seed = tgui::Label::create();
	genome_seed->setPosition(110, 110);
	genome_seed->setSize(100, 18);
	genome_seed->setTextSize(13);
	genome_seed->setText(to_string(GENOME_SEED));
	stats_panel->add(genome_seed);

	label = tgui::Label::create();
	label->setPosition(10, 130);
	label->setSize(110, 18);
	label->setTextSize(13);
	label->setText("Max generation:");
	stats_panel->add(label);
	auto max_generation = tgui::Label::create();
	max_generation->setPosition(120, 130);
	max_generation->setSize(100, 18);
	max_generation->setTextSize(13);
	max_generation->setText("1");
	stats_panel->add(max_generation);

	label = tgui::Label::create();
	label->setPosition(10, 150);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("Tics:");
	stats_panel->add(label);
	auto tics_counter = tgui::Label::create();
	tics_counter->setPosition(40, 150);
	tics_counter->setSize(200, 18);
	tics_counter->setTextSize(13);
	tics_counter->setText("0");
	stats_panel->add(tics_counter);

	label = tgui::Label::create();
	label->setPosition(10, 170);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("Entities:");
	stats_panel->add(label);
	auto entities_counter = tgui::Label::create();
	entities_counter->setPosition(65, 170);
	entities_counter->setSize(100, 18);
	entities_counter->setTextSize(13);
	entities_counter->setText("0");
	stats_panel->add(entities_counter);

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
	info_panel->add(cell_image_canvas);

	auto info_title = tgui::Label::create();
	info_title->setPosition(125, 10);
	info_title->setSize(100, 18);
	info_title->setTextSize(13);
	info_title->setText("cell");
	info_panel->add(info_title);

	label = tgui::Label::create();
	label->setPosition(72, 28);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("age:");
	info_panel->add(label);
	auto info_age = tgui::Label::create();
	info_age->setPosition(110, 28);
	info_age->setSize(100, 18);
	info_age->setTextSize(13);
	info_panel->add(info_age);

	label = tgui::Label::create();
	label->setPosition(10, 70);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("generation:");
	info_panel->add(label);
	auto info_generation = tgui::Label::create();
	info_generation->setPosition(85, 70);
	info_generation->setSize(100, 18);
	info_generation->setTextSize(13);
	info_panel->add(info_generation);

	label = tgui::Label::create();
	label->setPosition(10, 90);
	label->setSize(125, 18);
	label->setTextSize(13);
	label->setText("mutation chance:");
	info_panel->add(label);
	auto info_mutation_chance = tgui::Label::create();
	info_mutation_chance->setPosition(125, 90);
	info_mutation_chance->setSize(100, 18);
	info_mutation_chance->setTextSize(13);
	info_panel->add(info_mutation_chance);

	label = tgui::Label::create();
	label->setPosition(10, 110);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("energy:");
	info_panel->add(label);
	auto info_energy = tgui::Label::create();
	info_energy->setPosition(70, 110);
	info_energy->setSize(100, 18);
	info_energy->setTextSize(13);
	info_panel->add(info_energy);

	label = tgui::Label::create();
	label->setPosition(10, 130);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("organic power:");
	info_panel->add(label);
	auto info_organic_power = tgui::Label::create();
	info_organic_power->setPosition(110, 130);
	info_organic_power->setSize(100, 18);
	info_organic_power->setTextSize(13);
	info_panel->add(info_organic_power);

	label = tgui::Label::create();
	label->setPosition(10, 150);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("light power:");
	info_panel->add(label);
	auto info_light_power = tgui::Label::create();
	info_light_power->setPosition(110, 150);
	info_light_power->setSize(100, 18);
	info_light_power->setTextSize(13);
	info_panel->add(info_light_power);

	label = tgui::Label::create();
	label->setPosition(10, 170);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("genome:");
	info_panel->add(label);
	auto info_genome = tgui::Label::create();
	info_genome->setPosition(70, 170);
	info_genome->setSize(150, 40);
	info_genome->setTextSize(13);
	info_panel->add(info_genome);

	label = tgui::Label::create();
	label->setPosition(10, 190);
	label->setSize(100, 18);
	label->setTextSize(13);
	label->setText("genome args:");
	info_panel->add(label);
	auto info_genome_args = tgui::Label::create();
	info_genome_args->setPosition(100, 190);
	info_genome_args->setSize(150, 40);
	info_genome_args->setTextSize(13);
	info_panel->add(info_genome_args);

	auto SetInfoBox = [
		&hibernate, ecosys_ptr, info_genome, info_light_power,
		info_organic_power, info_mutation_chance,
		info_energy, info_generation,
		info_age, info_title,
		&cell_image, cell_image_canvas, info_genome_args, world_map
	](const Ecosystem::Logic::cell* cell)
	{
		if (hibernate)
		{
			return;
		}

		Color color = world_map.ObtainColor(cell);

		cell_image[0].color = color;
		cell_image[1].color = color;
		cell_image[2].color = color;
		cell_image[3].color = color;
		if (cell->ContainsEntity())
		{
			info_title->setText("Entity");
			info_genome->setText(to_string(cell->GetEntity()->GetGenome().Data()));
			info_generation->setText(to_string(cell->GetEntity()->GetGenome().Generation()));
			info_age->setText(to_string(cell->GetEntity()->Age()) + "/" + to_string(cell->GetEntity()->MaxAge()));
			info_mutation_chance->setText(to_string(cell->GetEntity()->GetGenome().MutationChance()));
			info_energy->setText(to_string(cell->GetEntity()->Energy()) + "/" + to_string(cell->GetEntity()->MaxEnergy()));
			info_genome_args->setText(to_string(cell->GetEntity()->GetGenome().Args()));
		}
		else
		{
			info_title->setText("Cell");
			info_genome->setText("-");
			info_generation->setText("-");
			info_age->setText("-");
			info_mutation_chance->setText("-");
			info_energy->setText("-");
			info_genome_args->setText("-");
		}

		if (cell->ContainsOrganic())
		{
			info_organic_power->setText(to_string(cell->GetOrganic()->Energy()));
		}
		else
		{
			info_organic_power->setText("-");
		}

		info_light_power->setText(to_string(cell->LightPower()));

		cell_image_canvas->clear();
		cell_image_canvas->draw(cell_image);
		cell_image_canvas->display();
	};

	canvas->onClick([SetInfoBox, ecosys_ptr](const sf::Vector2f& mousePos)
	{
		SetInfoBox(ecosys_ptr->GetCell(mousePos.x, mousePos.y));
	});

	auto ClearInfoBox = [
		&ecosys_ptr, info_genome, info_light_power,
		info_organic_power, info_mutation_chance, 
		info_energy, info_generation, 
		info_age, info_title,
		&cell_image, cell_image_canvas, info_genome_args]()
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
		info_organic_power->setText("-");
		info_light_power->setText("-");
		info_genome_args->setText("-");
	};

	ClearInfoBox();

	tgui::Button::Ptr info_clear_rect_btn = tgui::Button::create();
	info_clear_rect_btn->setPosition(80, 248);
	info_clear_rect_btn->setText("Clear");
	info_clear_rect_btn->onPress(ClearInfoBox);
	info_panel->add(info_clear_rect_btn, "info_clear");

	size_t tics = 0;
	unsigned int acum = 0;
	long long speedmeter = 0;
	while (main.isOpen())
	{
		Event event;
		while (main.pollEvent(event))
		{
			gui.handleEvent(event);

			if (event.type == sf::Event::Closed)
				main.close();

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::R:
					if (hibernate)
					{
						break;
					}
					pause = false;
					break;
				case sf::Keyboard::P:
					if (hibernate)
					{
						break;
					}
					pause = true;
					break;
				case sf::Keyboard::T:
					if (hibernate)
					{
						break;
					}
					pause = true;
					do_tic = true;
					break;
				}
			}
		}

		// Reload world if all entities die
		if (!world.Entities().size())
		{
			GENOME_SEED = random::Generate();
			Genome::Init(GENOME_SEED);
			world.Reload();
		}

		// logic
		if ((acum >= SPEED && !pause) || do_tic)
		{
			do_tic = false;

			auto start = high_resolution_clock::now();
			world.Update();
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<std::chrono::microseconds>(stop - start);
			if (duration.count() > speedmeter)
			{
				speedmeter = duration.count();
				cout << "World update max time: \t" << speedmeter << " ms\r";
			}

			tics++;
			acum = 0;
		}
		else
		{
			acum++;
		}
		
		// Drawing
		main.clear();

		if (!hibernate)
		{
			world_map.Draw(canvas);

			if (!pause)
			{
				tics_counter->setText(to_string(tics));
				max_generation->setText(to_string(world_map.GetMaxGeneration()));
				entities_counter->setText(to_string(world_map.GetEntitiesCount()));
			}
		}

		gui.draw();
		main.display();
	}

	return 0;
}

