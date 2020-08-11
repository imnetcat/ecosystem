#pragma once

#include "entity.h"
#include "cell.h"
#include <vector>
using namespace std;

struct EnvData
{
	vector<Entity> entity;
};

class Environment
{
public:
	explicit Environment(Entity*)
	{

	}

	EnvData Update();
private:
	Entity*
};
