#pragma once
#include "mineral.h"
#include "object.h"
#include "ration.h"
#include "entity.h"
#include "cell.h"
#include <vector>
#include <map>
#include <memory>
using namespace std;

const int ENVIRONMENT_SIZE_X = 800;
const int ENVIRONMENT_SIZE_Y = 500;

struct Position
{
	size_t x;
	size_t y;
};

bool operator < (const Position& lhs, const Position& rhs)
{
	return lhs.x < rhs.x && lhs.y < rhs.y;
}

using ObjectId = size_t;
using PositionId = size_t;

struct EntityState
{
	Position position;
	ObjectColor color;
};

class Environment
{
public:
	explicit Environment(size_t x, size_t y, shared_ptr<Object> e)
	{
		entities.push_back(e);
		positions.push_back({ x, y });
		front_position[positions.end() - 1] = entities.end() - 1;
		back_position[entities.end() - 1] = positions.end() - 1;
	}

	vector<EntityState> Update()
	{
		vector<EntityState> result;
		vector<shared_ptr<Object>>::iterator it = entities.begin();
		while (it != entities.end())
		{
			auto& entity = *it;
			Command command = entity->Tic();
			Event(command, back_position[it]);
						
			if (entity->Dying())
			{
				unsigned short residue = entity->Die();
				if (residue)
				{
					entity = shared_ptr<Object>(new Mineral(residue));
				}
				else
				{
					Erase(it);
				}
				continue;
			}

			EntityState stat; 
			stat.color = entity->Color();
			stat.position = *(back_position[it]);
			result.push_back(stat);
			it++;
		}
		return result;
	}
private:
	void Erase(vector<shared_ptr<Object>>::iterator id)
	{
		entities.erase(id);
		front_position.erase(back_position[id]);
		back_position.erase(id);
	}

	void Event(Command command, vector<Position>::iterator entity)
	{
		switch (command)
		{
		case move_left:
			if(entity->x > 0)
				entity->x -= CELL_SIZE;
			break;
		case move_right:
			if (entity->x < ENVIRONMENT_SIZE_X - 10)
				entity->x += CELL_SIZE;
			break;
		case move_bottom:
			if (entity->y < ENVIRONMENT_SIZE_Y - 10)
				entity->y += CELL_SIZE;
			break;
		case move_top:
			if (entity->y > 0)
				entity->y -= CELL_SIZE;
			break;
		default:
			break;
		}
	}
	
	vector<shared_ptr<Object>> entities;
	vector<Position> positions;
	map<vector<Position>::iterator, vector<shared_ptr<Object>>::iterator> front_position;
	map<vector<shared_ptr<Object>>::iterator, vector<Position>::iterator> back_position;
};
