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
const int ENVIRONMENT_SIZE_Y = 50;

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

struct Stat
{
	Position position;
	RGBColor color;
	bool outline;
	unsigned char shadow;
};

struct LightSource
{
	Position position;
	size_t power;
};
class Light
{
public:
	enum light_level
	{
		//surface = CELL_OUTLINE * 4,
		//shallow = CELL_OUTLINE * 4 +  1 * 8,
		//depth	= CELL_OUTLINE * 4 +  1 * 8,
		//abyss	= CELL_OUTLINE * 4 +  1 * 8 +  1 * 12,
		surface = CELL_OUTLINE,
		shallow = CELL_OUTLINE * 2,
		depth = CELL_OUTLINE,
		abyss = CELL_OUTLINE,
	};
};


class Environment
{
public:
	vector<LightSource> light_sources;
	explicit Environment()
	{
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				// set up light
				//if ((x != 0 || x != ENVIRONMENT_SIZE_X - 1) &&
				//	y == 0)
				//	light_sources.push_back({ {x, y},  LIGHT_LEVEL_POWER * 60 });
				if (x == ENVIRONMENT_SIZE_X/2 &&
					y == 0)
					light_sources.push_back({ {x, y},  LIGHT_LEVEL_POWER * 30 });

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

		auto x = ENVIRONMENT_SIZE_X / 2;
		auto y = ENVIRONMENT_SIZE_Y / 2;
		shared_ptr<Entity> e(new Cell(Gen({
			reproduction,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			reproduction,
			fotosintesis,
			fotosintesis,
			fotosintesis,
			stay,
			fotosintesis,
			stay,
			fotosintesis,
			fotosintesis,
			stay,
			reproduction,
			fotosintesis,
			}, 100)));

		// put first cells
		terrain[y][x]->SetEntity(e);
	}
	
	vector<Stat> Update()
	{
		vector<Stat> result;

		struct TicStat
		{
			Position position;
			size_t acc_energy = 0;
			std::array<Command, Gen::length> genom;
			size_t generation;
		};

		// gravitation effect
		size_t y = ENVIRONMENT_SIZE_Y - 2;
		while (true)
		{
			size_t x = ENVIRONMENT_SIZE_X - 1;
			while (true)
			{
				if (terrain[y][x]->IsContainsFood())
				{
					if (y < ENVIRONMENT_SIZE_Y)
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
				}
				if (x == 0)
					break;
				x--;
			}
			if (y == 0)
				break;
			y--;
		}


		TicStat tic_statistic;

		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				Shadow(y, x);
				vector<Command> commands;
				terrain[y][x]->Tic(commands);
				for (auto command : commands)
				{
					Event(command, x, y);
				}

				if (terrain[y][x]->IsContainsEntity())
				{
					if (terrain[y][x]->GetEntity()->AccEnergy() > tic_statistic.acc_energy)
					{
						tic_statistic.acc_energy = terrain[y][x]->GetEntity()->AccEnergy();
						tic_statistic.position = { x, y };
						tic_statistic.genom = terrain[y][x]->GetEntity()->GetGen().data;
						tic_statistic.generation = terrain[y][x]->GetEntity()->GetGen().generation;
					}
				}
			}
		}
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				terrain[y][x]->Untick();
				Stat stat;
				stat.color = terrain[y][x]->Color();
				stat.outline = terrain[y][x]->Outline();
				stat.shadow = terrain[y][x]->GetLightLevel();
				stat.position = { x * CELL_OUTLINE, y * CELL_OUTLINE };
				result.push_back(stat);
			}
		}

		if (tic_statistic.acc_energy)
		{
			cout << endl;
			cout << "\tGeneration:\t" << tic_statistic.generation << endl;
			cout << "\tPosition:\t" << tic_statistic.position.x << "  " << tic_statistic.position.y << endl;
			cout << "\tEnergy:\t" << tic_statistic.acc_energy << endl;
			cout << "\tGenom:\t";
			for (auto& g : tic_statistic.genom)
			{
				cout << g << ' ';
			}
			cout << endl;
		}

		return result;
	}
private:

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
			//if (distance * LIGHT_LEVEL_POWER < light.power)
				// свет источника достаёт до клетки
			//{
				// сила освещения в данной клетке
				//  данного источника света не считая препятствий
				size_t power = light.power;

				power_sum += power;
			//}
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

	void Event(const Command& command, const size_t& x, const size_t& y)
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
		case move_left:
			if (x > 0)
			{
				if (terrain[y][x - 1]->IsWalkable())
				{
					terrain[y][x - 1]->SetEntity(terrain[y][x]->GetEntity());
					terrain[y][x - 1]->GetEntity()->DecreaceEnergy(10);
					terrain[y][x]->DelEntity();
				}
			}
			break;
		case move_right:
			if (x < ENVIRONMENT_SIZE_X)
			{
				if (terrain[y][x + 1]->IsWalkable())
				{
					terrain[y][x + 1]->SetEntity(terrain[y][x]->GetEntity());
					terrain[y][x + 1]->GetEntity()->DecreaceEnergy(10);
					terrain[y][x]->DelEntity();
				}
			}
			break;
		case move_bottom:
			if (y < ENVIRONMENT_SIZE_Y)
			{
				if (terrain[y + 1][x]->IsWalkable())
				{
					terrain[y + 1][x]->SetEntity(terrain[y][x]->GetEntity());
					terrain[y + 1][x]->GetEntity()->DecreaceEnergy(10);
					terrain[y][x]->DelEntity();
				}
			}
			break;
		case move_top:
			if (y > 0)
			{
				if (terrain[y - 1][x]->IsWalkable())
				{
					terrain[y - 1][x]->SetEntity(terrain[y][x]->GetEntity());
					terrain[y - 1][x]->GetEntity()->DecreaceEnergy(10);
					terrain[y][x]->DelEntity();
				}
			}
			break;
		case fotosintesis:
		{
			terrain[y][x]->GetEntity()->IncreaceEnergy(terrain[y][x]->GetLightPower());
			//terrain[y][x]->GetEntity()->Eat(terrain[y][x]->GetFood().Eat(food));
		}
			break;
		case reproduction:
			if (terrain[y][x]->GetEntity()->AccEnergy() >= 200)
			{
				bool isSuccess = false;
				for (int i = 0; i < 5; i++)
				{
					Position new_position{ x, y };
					if (i == 0)
					{
						if (y < ENVIRONMENT_SIZE_Y)
						{
							new_position.y++;
						}
					}
					else if (i == 1)
					{
						if (new_position.y > 0)
						{
							new_position.y--;
						}
					}
					else if (i == 2)
					{
						if (x > 0)
						{
							new_position.x--;
						}
					}
					else if (i == 3)
					{
						if (x < ENVIRONMENT_SIZE_X)
						{
							new_position.x++;
						}
					}

					if (terrain[new_position.y][new_position.x]->IsWalkable())
					{
						terrain[y][x]->GetEntity()->DecreaceEnergy(200);
						terrain[new_position.y][new_position.x]->SetEntity(terrain[y][x]->GetEntity()->Reproduction());
						break;
					}
				}
			}
			break;
		}
	}

	std::array<std::array<shared_ptr<Structure>, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
};
