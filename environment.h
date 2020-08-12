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
const int ENVIRONMENT_SIZE_Y = CELL_SIZE * 50;

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
	enum light_level
	{
		surface = CELL_SIZE * 4,
		shallow = CELL_SIZE * 4 + CELL_SIZE * 8,
		depth	= CELL_SIZE * 4 + CELL_SIZE * 8,
		abyss	= CELL_SIZE * 4 + CELL_SIZE * 8 + CELL_SIZE * 12,
	};

	explicit Environment(size_t x, size_t y, shared_ptr<Entity> e)
	{
		entities.push_back(e);
		positions.push_back({ x, y });
		entities_front_position[positions.end() - 1] = entities.end() - 1;
		entities_back_position[entities.end() - 1] = positions.end() - 1;
	}

	vector<EntityState> UpdateEntities()
	{
		vector<EntityState> result;
		vector<shared_ptr<Entity>>::iterator it = entities.begin();
		while (it != entities.end())
		{
			auto& entity = *it;
			Command command = entity->Tic();
			Event(command, entities_back_position[it]);
						
			if (entity->Dying())
			{
				unsigned short residue = entity->Die();
				if (residue)
				{
					materials.push_back(shared_ptr<Object>(new Mineral(residue)));
					materials_front_position[entities_back_position[it]] = materials.end() - 1;
					materials_back_position[materials.end() - 1] = entities_back_position[it];
				}
				entities_front_position.erase(entities_back_position[it]);
				entities_back_position.erase(it);
				it = entities.erase(it);
				continue;
			}

			EntityState stat; 
			stat.color = entity->Color();
			stat.position = *(entities_back_position[it]);
			result.push_back(stat);
			it++;
		}
		return result;
	}
	vector<EntityState> UpdateMaterials()
	{
		vector<EntityState> result;
		vector<shared_ptr<Object>>::iterator it = materials.begin();
		while (it != materials.end())
		{
			auto& entity = *it;
			Command command = entity->Tic();
			Event(command, materials_back_position[it]);
			
			EntityState stat;
			stat.color = entity->Color();
			stat.position = *(materials_back_position[it]);
			result.push_back(stat);
			it++;
		}
		return result;
	}
private:
	void Erase(vector<shared_ptr<Entity>>::iterator id)
	{
		positions.erase(entities_back_position[id]);
		entities_front_position.erase(entities_back_position[id]);
		entities_back_position.erase(id);
		id = entities.erase(id);
	}
	void Erase(vector<shared_ptr<Object>>::iterator id)
	{
		positions.erase(materials_back_position[id]);
		materials_front_position.erase(materials_back_position[id]);
		materials_back_position.erase(id);
		id = materials.erase(id);
	}

	void Event(Command command, vector<Position>::iterator entity)
	{
		switch (command)
		{
		case skip:
			break;
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
		case fotosintesis:
			auto& ration = (*entities_front_position[entity])->GetRation();
			if (ration.fotosintesis > ration.entities && ration.entities < 10)
			{
				(*entities_front_position[entity])->Fotosintesis(Fotosintesis(entity->y));
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
	
	vector<shared_ptr<Entity>> entities;
	map<vector<Position>::iterator, vector<shared_ptr<Entity>>::iterator> entities_front_position;
	map<vector<shared_ptr<Entity>>::iterator, vector<Position>::iterator> entities_back_position;

	vector<shared_ptr<Object>> materials;
	map<vector<Position>::iterator, vector<shared_ptr<Object>>::iterator> materials_front_position;
	map<vector<shared_ptr<Object>>::iterator, vector<Position>::iterator> materials_back_position;
};
