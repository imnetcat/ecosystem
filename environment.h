#pragma once

#include "cell.h"
#include "glass.h"
#include "water.h"

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
#include <cmath>

using namespace std;

const int ENVIRONMENT_SIZE_X = 50;
const int ENVIRONMENT_SIZE_Y = 120;

struct Position
{
	size_t x;
	size_t y;
};

bool operator == (const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

using CellId = size_t;
using ObjectId = size_t;
using PositionId = size_t;

struct DrawData
{
	Position position;
	RGBColor color;
	bool outline;
	unsigned char shadow;
};

struct Info
{
	RGBColor color;
	struct Age
	{
		size_t curr = 0;
		size_t max = 0;
	} age;
	vector<Command> genom;
	unsigned short hp = 0;
	size_t energy = 0;
	size_t acc_energy = 0;
	size_t generation = 0;
	size_t light_power = 0;
	size_t food_power = 0;
};

struct LightLevel
{
	size_t power;
	size_t power_max;
};

class Environment
{
public:
	explicit Environment()
	{
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				// set up structures
				if ((x == 0 || x == ENVIRONMENT_SIZE_X - 1) ||
					(y == ENVIRONMENT_SIZE_Y - 1))
				{
					terrain[y][x] = shared_ptr<Structure>(new Glass());
				}
				else
				{
					terrain[y][x] = shared_ptr<Structure>(new Water());
				}

				// set up light
				size_t power = LIGHT_POWER;
				if (y != 0)
					power = terrain[y - 1][x]->Transparency() * terrain[y - 1][x]->GetLightPower();

				terrain[y][x]->SetLightPower(power);
			}
		}

		auto x = ENVIRONMENT_SIZE_X / 2;
		auto y = ENVIRONMENT_SIZE_Y / 8;
		shared_ptr<Entity> e(new Cell(Gen({
			reproduction,
			turn_right,
			fotosintesis,
			attack,
			reproduction,
			fotosintesis,
			symbiosis,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			symbiosis,
			fotosintesis,
			fotosintesis,
			reproduction,
			fotosintesis,
			reproduction,
			attack,
			attack,
			symbiosis,
			reproduction,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			eat_minerals,
			fotosintesis,
			eat_minerals,
			fotosintesis,
			eat_minerals,
			turn_right,
			reproduction,
			fotosintesis
			}, 0.2), Ration(), 0));

		// put first cells
		terrain[y][x]->SetEntity(e);
	}
	
	void Update()
	{
		// gravitation effect
		size_t y = ENVIRONMENT_SIZE_Y - 2;
		while (true)
		{
			size_t x = ENVIRONMENT_SIZE_X - 1;
			while (true)
			{
				if (terrain[y][x]->IsContainsFood())
				{
					if (terrain[y + 1][x]->IsWalkable())
					{
						if (!terrain[y + 1][x]->IsContainsFood())
						{
							terrain[y + 1][x]->SetFood(terrain[y][x]->GetFood());
							terrain[y][x]->DelFood();
						}
					}
				}
				if (x == 0)
					break;
				x--;
			}
			if (y == 0)
				break;
			y--;
		}

		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				vector<Command> commands;
				terrain[y][x]->Tic(commands);
				for (auto command : commands)
				{
					Event(command, x, y);
				}

				// set up light
				size_t power = LIGHT_POWER;
				if(y != 0)
					power = terrain[y - 1][x]->Transparency() * terrain[y - 1][x]->GetLightPower();

				terrain[y][x]->SetLightPower(power);
			}
		}
	}

	vector<DrawData> UpdateColors()
	{
		vector<DrawData> result;
		
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				terrain[y][x]->Untick();
				DrawData stat;
				stat.color = terrain[y][x]->Color(view);
				stat.outline = terrain[y][x]->Outline(view);
				if (view == view_settings::terrain)
					stat.shadow = terrain[y][x]->GetLightLevel();
				else
					stat.shadow = 0;
				stat.position = { x * CELL_OUTLINE, y * CELL_OUTLINE };
				result.push_back(stat);
			}
		}

		return result;
	}

	Info GetInfo(size_t x_px, size_t y_px)
	{
		size_t x = x_px / CELL_OUTLINE;
		size_t y = y_px / CELL_OUTLINE;
		Info info;
		info.color = terrain[y][x]->Color(view);
		info.light_power = terrain[y][x]->GetLightPower();
		if (terrain[y][x]->IsContainsEntity())
		{
			info.age.curr = terrain[y][x]->GetEntity()->Age();
			info.age.max = MAX_CELL_AGE;
			auto& data = terrain[y][x]->GetEntity()->GetGen().data;
			for (auto command : data)
			{
				info.genom.push_back(command);
			}
			info.generation = terrain[y][x]->GetEntity()->GetGen().generation;
			info.hp = terrain[y][x]->GetEntity()->Hp();
			info.energy = terrain[y][x]->GetEntity()->Energy();
			info.acc_energy = terrain[y][x]->GetEntity()->AccEnergy();
		}
		if (terrain[y][x]->IsContainsFood())
		{
			info.food_power = terrain[y][x]->GetFood().Get();
		}
 		return info;
	}

	view_settings GetView()
	{
		return view;
	}
	void SetView(view_settings new_val)
	{
		view = new_val;
	}
private:

	view_settings view = view_settings::terrain;
		
	void Event(Command command, size_t x, size_t y)
	{
		switch (command)
		{
		case Command::stay:
			terrain[y][x]->GetEntity()->DecreaceEnergy(5);
			break;
		case Command::die:
			if (terrain[y][x]->IsContainsFood())
				terrain[y][x]->GetFood().Put(terrain[y][x]->GetEntity()->AccEnergy() + 100);
			else
				terrain[y][x]->SetFood(terrain[y][x]->GetEntity()->AccEnergy() + 100);

			terrain[y][x]->DelEntity();
			break;
		case Command::move:
			Position new_position = GetViewedPosition(terrain[y][x]->GetEntity()->GetView(), { x, y });

			if (new_position == Position{ x, y })
				break;

			if (terrain[new_position.y][new_position.x]->IsWalkable() && !terrain[new_position.y][new_position.x]->IsContainsEntity())
			{
				terrain[new_position.y][new_position.x]->SetEntity(terrain[y][x]->GetEntity());
				terrain[new_position.y][new_position.x]->GetEntity()->DecreaceEnergy(10);
				terrain[y][x]->DelEntity();
			}
			break;
		case Command::turn_left:
		{
			unsigned short old_side = terrain[y][x]->GetEntity()->GetView();
			view_side new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
			terrain[y][x]->GetEntity()->SetView(new_side);
			terrain[y][x]->GetEntity()->DecreaceEnergy(8);
		}
			break;
		case Command::turn_right:
		{
			unsigned short old_side = terrain[y][x]->GetEntity()->GetView();
			view_side new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
			terrain[y][x]->GetEntity()->SetView(new_side);
			terrain[y][x]->GetEntity()->DecreaceEnergy(8);
		}
			break;
		case Command::eat_minerals:
		{
			terrain[y][x]->GetEntity()->DecreaceEnergy(10);
			auto energy = terrain[y][x]->GetFood().Eat(6000);
			if (energy)
			{
				terrain[y][x]->GetEntity()->GetRation().IncreaceMinerals();
				terrain[y][x]->GetEntity()->IncreaceEnergy(energy);
			}
		}
			break;
		case Command::fotosintesis:
		{
			terrain[y][x]->GetEntity()->DecreaceEnergy(10);
			auto energy = terrain[y][x]->GetLightPower();
			if (energy)
			{
				terrain[y][x]->GetEntity()->GetRation().IncreaceLight();
				terrain[y][x]->GetEntity()->IncreaceEnergy(energy);
			}
		}
			break;
		case Command::symbiosis:
		{
			if (terrain[y][x]->GetEntity()->AccEnergy() < 400)
				break;

			Position recipient_position = GetViewedPosition(terrain[y][x]->GetEntity()->GetView(), { x, y });

			if (recipient_position == Position{ x, y })
				break;

			if (terrain[recipient_position.y][recipient_position.x]->IsContainsEntity())
			{
				if (terrain[recipient_position.y][recipient_position.x]->GetEntity()->IsFriendly(terrain[y][x]->GetEntity()->GetGen()))
				{
					terrain[y][x]->GetEntity()->DecreaceAccEnergy(400);
					terrain[recipient_position.y][recipient_position.x]->GetEntity()->IncreaceEnergy(400);
					terrain[recipient_position.y][recipient_position.x]->GetEntity()->GetRation().Symbiosis();
				}
			}
		}
		break;
		case Command::attack:
		{
			Position enemy_position = GetViewedPosition(terrain[y][x]->GetEntity()->GetView(), { x, y });

			if (enemy_position == Position{ x, y })
				break;

			if (terrain[enemy_position.y][enemy_position.x]->IsContainsEntity())
			{
				if (terrain[enemy_position.y][enemy_position.x]->GetEntity()->IsFriendly(terrain[y][x]->GetEntity()->GetGen()))
				{
					Event(die, enemy_position.x, enemy_position.y);
					terrain[y][x]->GetEntity()->IncreaceEnergy(terrain[enemy_position.y][enemy_position.x]->GetFood().Eat());
					terrain[y][x]->GetEntity()->GetRation().IncreaceMeat();
				}
			}
		}
			break;
		case Command::reproduction:
		{
			if (terrain[y][x]->GetEntity()->AccEnergy() < 200)
				break;

			Position new_position = GetViewedPosition(terrain[y][x]->GetEntity()->GetView(), { x,y });

			if (new_position == Position{ x, y })
				break;

			if (terrain[new_position.y][new_position.x]->IsWalkable())
			{
				terrain[y][x]->GetEntity()->DecreaceAccEnergy(200);
				terrain[new_position.y][new_position.x]->SetEntity(terrain[y][x]->GetEntity()->Reproduction());
				break;
			}
		}
			break;
		}
	}

	Position GetViewedPosition(view_side view, Position init)
	{
		Position viewed_position = init;
		switch (view)
		{
		case view_side::left:
			if (init.x > 0)
			{
				viewed_position.x--;
			}
			break;
		case view_side::right:
			if (init.x < ENVIRONMENT_SIZE_X)
			{
				viewed_position.x++;
			}
			break;
		case view_side::bottom:
			if (init.y > 0)
			{
				viewed_position.y--;
			}
			break;
		case view_side::top:
			if (init.y < ENVIRONMENT_SIZE_Y)
			{
				viewed_position.y++;
			}
			break;
		case view_side::left_bottom:
			if (init.x > 0 && init.y > 0)
			{
				viewed_position.x--;
				viewed_position.y++;
			}
			break;
		case view_side::left_top:
			if (init.x > 0 && init.y > 0)
			{
				viewed_position.x--;
				viewed_position.y--;
			}
			break;
		case view_side::right_bottom:
			if (init.y < ENVIRONMENT_SIZE_Y && init.x < ENVIRONMENT_SIZE_X)
			{
				viewed_position.y++;
				viewed_position.x++;
			}
			break;
		case view_side::right_top:
			if (init.y > 0 && init.x < ENVIRONMENT_SIZE_X)
			{
				viewed_position.y--;
				viewed_position.x++;
			}
			break;
		}
		return viewed_position;
	}

	std::array<std::array<shared_ptr<Structure>, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
};
