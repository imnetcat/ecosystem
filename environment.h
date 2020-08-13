#pragma once
#include "mineral.h"
#include "structure.h"
#include "ration.h"
#include "cell.h"
#include "glass.h"
#include "water.h"
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
#include <array>
using namespace std;

const int ENVIRONMENT_SIZE_X = 36;
const int ENVIRONMENT_SIZE_Y = 16;

struct Position
{
	size_t x;
	size_t y;
};

bool operator == (const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}


#define NAC shared_ptr<Cell>(nullptr) // Not A Cell
#define NAM shared_ptr<Resource>(nullptr) // Not A Material
using CellId = size_t;
using ObjectId = size_t;
using PositionId = size_t;

struct Stat
{
	Position position;
	RGBColor color;
	bool outline;
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
	vector<LightSource> light_sources = {
		//{{ 0, 0 }, CELL_OUTLINE * 4},
		//{{ 0, 0 }, CELL_OUTLINE * 5},
	};
	explicit Environment(size_t x, size_t y, shared_ptr<Cell> e)
	{
		for (size_t y = 0; y < ENVIRONMENT_SIZE_Y; y++)
		{
			for (size_t x = 0; x < ENVIRONMENT_SIZE_X; x++)
			{
				if ((x == 0 || x == ENVIRONMENT_SIZE_X - 1) ||
					(y == ENVIRONMENT_SIZE_Y - 1))
				{
					terrain[y][x] = shared_ptr<Structure>(new Glass(1));
				}
				else
				{
					terrain[y][x] = shared_ptr<Structure>(new Water(0));
				}
			}
		}

		terrain[y][x]->SetEntity(e);
	}
	
	vector<Stat> Update()
	{
		vector<Stat> result;
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
				Stat stat;
				stat.color = terrain[y][x]->Color();
				stat.outline = terrain[y][x]->Outline();
				stat.position = { x * CELL_OUTLINE, y * CELL_OUTLINE };
				result.push_back(stat);
			}
		}
		return result;
	}
private:

	void Event(const Command& command, const size_t& x, const size_t& y)
	{
		switch (command)
		{
		case skip:
			break;
		case gravity:
			if (y < ENVIRONMENT_SIZE_Y)
			{
				if (terrain[y + 1][x]->IsWalkable())
				{
					if (terrain[y][x]->IsContainsEntity())
					{
						terrain[y + 1][x]->SetEntity(terrain[y][x]->GetEntity());
						terrain[y][x]->DelEntity();
					}
					if (terrain[y][x]->IsContainsStruct())
					{
						terrain[y + 1][x]->SetStruct(terrain[y][x]->GetStruct());
						terrain[y][x]->DelStruct();
					}
				}
			}
			break;
		case die:
			terrain[y][x]->SetStruct(shared_ptr<Structure>(new Mineral(100)));
			terrain[y][x]->DelEntity();
			break;
		case move_left:
			if (x > 0)
			{
				if (terrain[y][x - 1]->IsWalkable())
				{
					terrain[y][x - 1]->SetEntity(terrain[y][x]->GetEntity());
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
					terrain[y][x]->DelEntity();
				}
			}
			break;
		case fotosintesis:
		{
			const auto& ration = terrain[y][x]->GetEntity()->GetRation();
			if (ration.fotosintesis > ration.entities && ration.entities < 10)
			{
				terrain[y][x]->GetEntity()->Fotosintesis(Fotosintesis(terrain[y][x]->GetLightLevel()));
			}
		}
			break;
		case furcation:
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
						terrain[new_position.y][new_position.x]->SetEntity(terrain[y][x]->GetEntity()->Furcation());
						break;
					}
				}
			}
			break;
		}
	}

	unsigned short Fotosintesis(unsigned short light_level)
	{
		const short koef = 20;
		return koef * light_level;
	}
	std::array<std::array<shared_ptr<Structure>, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
};
