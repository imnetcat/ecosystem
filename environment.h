#pragma once
#include "mineral.h"
#include "object.h"
#include "ration.h"
#include "cell.h"
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <ctime>
using namespace std;

const int ENVIRONMENT_SIZE_X = CELL_OUTLINE * 8;
const int ENVIRONMENT_SIZE_Y = CELL_OUTLINE * 6;

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

struct CellState
{
	Position position;
	ObjectColor color;
};

class Environment
{
public:
	enum light_level
	{
		//surface = CELL_OUTLINE * 4,
		//shallow = CELL_OUTLINE * 4 + CELL_OUTLINE * 8,
		//depth	= CELL_OUTLINE * 4 + CELL_OUTLINE * 8,
		//abyss	= CELL_OUTLINE * 4 + CELL_OUTLINE * 8 + CELL_OUTLINE * 12,
		surface = CELL_OUTLINE,
		shallow = CELL_OUTLINE * 2,
		depth	= CELL_OUTLINE,
		abyss	= CELL_OUTLINE,
	};

	explicit Environment(size_t x, size_t y, shared_ptr<Cell> e)
	{
		entities.push_back(e);
		positions.push_back({ x, y });
		entities_front_position[positions.size() - 1] = entities.size() - 1;
		entities_back_position[entities.size() - 1] = positions.size() - 1;
	}

	vector<CellState> UpdateEntities()
	{
		vector<CellState> result;
		auto it = entities.begin();
		for (size_t i = 0; i < entities.size(); i++)
		{
			//auto& entity = entities[i];
			Command command = entities[i]->Tic();
			Event(command, entities_back_position[i]);
						
			if (entities[i]->Dying())
			{
				unsigned short residue = entities[i]->Die();
				if (residue)
				{
					materials.push_back(shared_ptr<Object>(new Mineral(residue)));
					materials_front_position[entities_back_position[i]] = materials.size() - 1;
					materials_back_position[materials.size() - 1] = entities_back_position[i];
				}
				entities_front_position.erase(entities_back_position[i]);
				entities_back_position.erase(i);
				it = entities.erase(it);
				if(it != entities.end())
					it++;
				continue;
			}

			CellState stat; 
			stat.color = entities[i]->Color();
			stat.position = positions[entities_back_position[i]];
			result.push_back(stat);
		}
		return result;
	}

	vector<CellState> UpdateMaterials()
	{
		vector<CellState> result;
		auto it = materials.begin();
		for (size_t i = 0; i < materials.size(); i++)
		{
			auto& material = *it;
			Command command = material->Tic();
			Event(command, materials_back_position[i]);
			
			CellState stat;
			stat.color = material->Color();
			stat.position = positions[materials_back_position[i]];
			result.push_back(stat);
			it++;
		}
		return result;
	}
private:

	void Event(Command command, PositionId entity)
	{
		switch (command)
		{
		case skip:
			break;
		case move_left:
			if (positions[entity].x > 0)
			{
				auto it = find(positions.begin(), positions.end(), Position{ positions[entity].x - CELL_OUTLINE, positions[entity].y});
				if (it == positions.end())
				{
					entities[entities_front_position[entity]]->SetViewPoint(view_point::left);
					positions[entity].x -= CELL_OUTLINE;
				}
			}
			break;
		case move_right:
			if (positions[entity].x < ENVIRONMENT_SIZE_X - CELL_OUTLINE)
			{
				auto it = find(positions.begin(), positions.end(), Position{ positions[entity].x + CELL_OUTLINE, positions[entity].y });
				if (it == positions.end())
				{
					entities[entities_front_position[entity]]->SetViewPoint(view_point::right);
					positions[entity].x += CELL_OUTLINE;
				}
			}
			break;
		case move_bottom:
			if (positions[entity].y < ENVIRONMENT_SIZE_Y - CELL_OUTLINE)
			{
				auto it = find(positions.begin(), positions.end(), Position{ positions[entity].x, CELL_OUTLINE + positions[entity].y });
				if (it == positions.end())
				{
					entities[entities_front_position[entity]]->SetViewPoint(view_point::bottom);
					positions[entity].y += CELL_OUTLINE;
				}
			}
			break;
		case move_top:
			if (positions[entity].y > 0)
			{
				auto it = find(positions.begin(), positions.end(), Position{ positions[entity].x, positions[entity].y - CELL_OUTLINE });
				if (it == positions.end())
				{
					entities[entities_front_position[entity]]->SetViewPoint(view_point::top);
					positions[entity].y -= CELL_OUTLINE;
				}
			}
			break;
		case fotosintesis:
		{
			auto& ration = entities[entities_front_position[entity]]->GetRation();
			if (ration.fotosintesis > ration.entities && ration.entities < 10)
			{
				entities[entities_front_position[entity]]->Fotosintesis(Fotosintesis(positions[entity].y));
			}
		}
			break;
		case furcation:
			if (entities[entities_front_position[entity]]->AccEnergy() >= 200)
			{
				view_point vpoint = entities[entities_front_position[entity]]->GetViewPoint();
				view_point new_vpoint = vpoint;
				bool isSuccess = false;
				for (int i = -1; i < 4; i++)
				{
					if (i != -1)
						vpoint = static_cast<view_point>(i);

					Position position{ positions[entity].x, positions[entity].y };
					if (vpoint == view_point::bottom)
					{
						if (positions[entity].y < ENVIRONMENT_SIZE_Y - CELL_OUTLINE)
						{
							new_vpoint = view_point::bottom;
							position.y += CELL_OUTLINE;
						}
					}
					if (vpoint == view_point::top)
					{
						if (position.y > 0)
						{
							new_vpoint = view_point::top;
							position.y -= CELL_OUTLINE;
						}
					}
					if (vpoint == view_point::left)
					{
						if (positions[entity].x > 0)
						{
							new_vpoint = view_point::left;
							position.x -= CELL_OUTLINE;
						}
					}
					if (vpoint == view_point::right)
					{
						if (positions[entity].x < ENVIRONMENT_SIZE_X - CELL_OUTLINE)
						{
							new_vpoint = view_point::right;
							position.x += CELL_OUTLINE;
						}
					}
					auto it = find(positions.begin(), positions.end(), position);
					if (positions.end() == it)
					{
						auto new_cell = entities[entities_front_position[entity]]->Furcation();
						entities.push_back(new_cell);
						positions.push_back(position);
						entities_front_position[positions.size() - 1] = entities.size() - 1;
						entities_back_position[entities.size() - 1] = positions.size() - 1;
						isSuccess = true;
						entities[entities_front_position[positions.size() - 1]]->SetViewPoint(new_vpoint);
						entities[entities_front_position[entity]]->SetViewPoint(new_vpoint);
						break;
					}
				}
				if (isSuccess)
				{
					entities[entities_front_position[entity]]->DecreaceAccEnergy(200);
				}
			}
			break;
		}
	}

	unsigned short Fotosintesis(size_t y)
	{
		if (y < light_level::surface)
		{
			return 20;
		}
		else if (y < light_level::shallow)
		{
			return 15;
		}
		else if (y < light_level::depth)
		{
			return 10;
		}
		else if (y < light_level::abyss)
		{
			return 5;
		}
		else
		{
			return 0;
		}
	}

	vector<Position> positions;
	
	vector<shared_ptr<Cell>> entities;
	map<PositionId, CellId> entities_front_position;
	map<CellId, PositionId> entities_back_position;

	vector<shared_ptr<Object>> materials;
	map<ObjectId, PositionId> materials_front_position;
	map<PositionId, ObjectId> materials_back_position;
};
