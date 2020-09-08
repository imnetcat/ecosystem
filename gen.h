#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include "object.h"

enum Command
{
	die,
	stay,
	move,
	turn_left,
	turn_right,
	fotosintesis,
	eat_minerals,
	attack, // атака клетки если её геном отличается более чем на 2 комманды
	reproduction,  // деление на две
	symbiosis
};

class Gen
{
public:
	static const unsigned char length = 32;
	static const size_t commands = 9;
	Gen() : generation(0), mutationChance((rand() % 100) / (double)100), hash(Hashing()), index(0)
	{
		srand(time(0) - rand());
		for (size_t i = 0; i < length; i++)
		{
			Command cmd = static_cast<Command>(rand() % Gen::commands);
			data[i] = cmd == die ? stay : cmd;
		}
	};
	explicit Gen(std::array<Command, length> d, double mh, size_t g = 1)
		: generation(g), mutationChance(mh), data(d), hash(Hashing()), index(0) {};

	size_t generation;
	double mutationChance;
	std::array<Command, length> data;

	RGBColor Hash()
	{
		return hash;
	}

	Command Read()
	{
		if (index + 1 == data.size())
		{
			index = 0;
			return data[data.size() - 1];
		}
		else
			return data[index++];
	}
private:
	RGBColor Hashing()
	{
		std::stringstream rs;
		unsigned char g = 0;
		unsigned char b = 0;
		for (unsigned short i = 0; i < Gen::length; i++)
		{
			unsigned char gen = static_cast<unsigned char>(data[i])* ((double)255 / (Gen::length * Gen::commands));
			rs << gen;
			g += gen * (((double)255 / Gen::length) * Gen::commands);
			b += gen * ((double)255 / (Gen::length * Gen::commands));
		}

		unsigned char r = std::hash<std::string>{}(rs.str()) % 255;
		return { r, g, b };
	}
	const RGBColor hash;
	// указатель текущей команды
	size_t index = 0;
};
