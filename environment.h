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

const int ENVIRONMENT_SIZE_X = 30;
const int ENVIRONMENT_SIZE_Y = 80;

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

struct LightSource
{
	Position position;
	size_t power;
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
				// set up light
				if ((x != 0 || x != ENVIRONMENT_SIZE_X - 1) &&
					y == 0)
					light_sources.push_back({ {x, y},  LIGHT_LEVEL_POWER * 30 });
				//if (x == ENVIRONMENT_SIZE_X/2 &&
				//	y == 0)
				//	light_sources.push_back({ {x, y},  LIGHT_LEVEL_POWER * 40 });

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
			}
		}
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				Shadow(y, x);
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
			minerals,
			fotosintesis,
			minerals,
			fotosintesis,
			minerals,
			turn_right,
			reproduction,
			fotosintesis
			}, 0.2), ration(), 0));

		// put first cells
		terrain[y][x]->SetEntity(e);
	}
	
	vector<DrawData> Update()
	{
		vector<DrawData> result;

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
			}
		}
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				terrain[y][x]->Untick();
				DrawData stat;
				stat.color = terrain[y][x]->Color();
				stat.outline = terrain[y][x]->Outline();
				stat.shadow = terrain[y][x]->GetLightLevel();
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
		info.color = terrain[y][x]->Color();
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

private:

	vector<LightSource> light_sources;

	void ShadowVector(Position source, Position node, size_t& power)
	{
		// определяем с какой стороны от
		//  данной точки находится источник света
		
		// * - источник
		// 0 - данная точка

		// *
		//   
		//    0
		if (source.x < node.x && source.y < node.y)
		{
			ShadowVector(source, { node.x - 1, node.y - 1 }, power);
			power *= terrain[node.y - 1][node.x - 1]->Transparency();
		}
		//     *
		//   
		// 0
		else if (source.x > node.x && source.y < node.y)
		{
			ShadowVector(source, { node.x + 1, node.y - 1 }, power);
			power *= terrain[node.y - 1][node.x + 1]->Transparency();
		}
		//     0
		//   
		// *
		else if (source.x < node.x && source.y > node.y)
		{
			ShadowVector(source, { node.x - 1, node.y + 1 }, power);
			power *= terrain[node.y + 1][node.x - 1]->Transparency();
		}
		// 0
		//   
		//     *
		else if (source.x > node.x && source.y > node.y)
		{
			ShadowVector(source, { node.x + 1, node.y + 1 }, power);
			power *= terrain[node.y + 1][node.x + 1]->Transparency();
		}
		// 0   *
		else if (source.x > node.x && source.y == node.y)
		{
			ShadowVector(source, { node.x + 1, node.y }, power);
			power *= terrain[node.y][node.x + 1]->Transparency();
		}
		// *   0
		else if (source.x < node.x && source.y == node.y)
		{
			ShadowVector(source, { node.x - 1, node.y }, power);
			power *= terrain[node.y][node.x - 1]->Transparency();
		}
		//   0
		//   
		//   *
		else if (source.x == node.x && source.y > node.y)
		{
			ShadowVector(source, { node.x, node.y + 1 }, power);
			power *= terrain[node.y + 1][node.x]->Transparency();
		}
		//   *
		//   
		//   0
		else if (source.x == node.x && source.y < node.y)
		{
			ShadowVector(source, { node.x, node.y - 1 }, power);
			power *= terrain[node.y - 1][node.x]->Transparency();
		}
	}

	size_t LightPower(size_t y, size_t x)
	{
		size_t power_sum = 0;
		for (auto& light : light_sources)
		{
			// определяем может ли достать
			//  источник освещения до клетки
			size_t dx = max(light.position.x, x) - min(light.position.x, x);
			size_t dy = max(light.position.y, y) - min(light.position.y, y);
			size_t distance = sqrt(dx * dx + dy * dy);
			if (distance  * LIGHT_LEVEL_POWER < light.power)
				// свет источника достаёт до клетки
			{
				// сила освещения в данной клетке
				//  данного источника света не считая препятствий
				size_t power = light.power;
				if (!power)
					continue;

				// учитываем прозрачность препятствий
				ShadowVector({ light.position.x, light.position.y } , { x, y }, power);
				if (!power)
					continue;

				power_sum += power;
			}
		}
		return power_sum;
	}
	size_t MaxLightPower(size_t y, size_t x)
	{
		size_t power_sum = 0;
		for (auto& light : light_sources)
		{
			// определяем может ли достать
			//  источник освещения до клетки
			size_t dx = max(light.position.x, x) - min(light.position.x, x);
			size_t dy = max(light.position.y, y) - min(light.position.y, y);
			size_t distance = sqrt(dx * dx + dy * dy);
			// сила освещения в данной клетке
			//  данного источника света не считая препятствий
			size_t power = light.power;

			power_sum += power;
		}
		return power_sum;
	}



	void Shadow(size_t y, size_t x)
	{
		auto lp = LightPower(y, x);
		float power = (float)lp / MaxLightPower(y, x);
		auto lv = 128 - power * 128;
		terrain[y][x]->SetLightPower(lp);
		terrain[y][x]->SetLightLevel(lv);
	}

	void Event(Command command, size_t x, size_t y)
	{
		switch (command)
		{
		case stay:
			terrain[y][x]->GetEntity()->DecreaceEnergy(5);
			break;
		case die:
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
		case turn_left:
		{
			unsigned short old_side = terrain[y][x]->GetEntity()->GetView();
			view_side new_side = static_cast<view_side>(old_side == 0 ? 7 : old_side - 1);
			terrain[y][x]->GetEntity()->SetView(new_side);
			terrain[y][x]->GetEntity()->DecreaceEnergy(8);
		}
			break;
		case turn_right:
		{
			unsigned short old_side = terrain[y][x]->GetEntity()->GetView();
			view_side new_side = static_cast<view_side>(old_side == 7 ? 0 : old_side + 1);
			terrain[y][x]->GetEntity()->SetView(new_side);
			terrain[y][x]->GetEntity()->DecreaceEnergy(8);
		}
			break;
		case minerals:
		{
			terrain[y][x]->GetEntity()->DecreaceEnergy(10);
			auto energy = terrain[y][x]->GetFood().Eat(6000);
			if (energy)
			{
				terrain[y][x]->GetEntity()->Ration().IncreaceMinerals();
				terrain[y][x]->GetEntity()->IncreaceEnergy(energy);
			}
		}
			break;
		case fotosintesis:
		{
			terrain[y][x]->GetEntity()->DecreaceEnergy(LIGHT_LEVEL_POWER);
			auto energy = terrain[y][x]->GetLightPower();
			if (energy)
			{
				terrain[y][x]->GetEntity()->Ration().IncreaceLight();
				terrain[y][x]->GetEntity()->IncreaceEnergy(energy);
			}
		}
			break;
		case symbiosis:
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
					terrain[recipient_position.y][recipient_position.x]->GetEntity()->Ration().Symbiosis();
				}
			}
		}
		break;
		case attack:
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
					terrain[y][x]->GetEntity()->Ration().IncreaceMeat();
				}
			}
		}
			break;
		case reproduction:
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
