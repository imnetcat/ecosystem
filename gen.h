#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include "object.h"

class Gen
{
public:
	enum Command
	{
		die,
		stay,
		move,
		turn_left,
		turn_right,
		eat,
		separation, // деление на две
		birth, // рождение
	};
public:
	Gen()
	{
		// выбираем к какой группе будет принадлежать ген
		switch (static_cast<CommandGroups>(rand() % CommandGroupsCount))
		{
		case CommandGroups::Movei:
		{
			// выбираем к какой группе будет принадлежать ген
			switch (static_cast<Moving>(rand() % MoveCount))
			{
			case Moving::Stay:
			{
				command = stay;
				break;
			}
			case Moving::Move:
			{
				command = move;
				break;
			}
			case Moving::Turn:
			{
				switch (static_cast<Turning>(rand() % TurnCount))
				{
				case Left:
				{
					command = turn_left;
					break;
				}
				case Right:
				{
					command = turn_right;
					break;
				}
				}
				break;
			}
			}
			break;
		}
		case CommandGroups::Food:
		{
			command = eat;
			break;
		}
		case CommandGroups::Reproduction:
		{
			switch (static_cast<Reproductioning>(rand() % ReproductionCount))
			{
			case Separation:
			{
				command = separation;
				break;
			}
			case Birth:
			{
				command = birth;
				break;
			}
			}
			break;
		}
		}
	}

	Command Read() const
	{
		return command;
	}

private:
	Command command;

	static const unsigned short CommandGroupsCount = 3;
	static const unsigned short MoveCount = 3;
	static const unsigned short TurnCount = 2;
	static const unsigned short FoodCount = 3;
	static const unsigned short ReproductionCount = 2;

	enum CommandGroups
	{
		Movei,
		Food,
		Reproduction
	};
	enum Moving
	{
		Stay,
		Move,
		Turn
	};
	enum Turning
	{
		Left,
		Right
	};
	enum Reproductioning
	{
		Separation, // деление на две
		Birth, // рождение
	};
};

class Genome
{
public:
	static const unsigned char length = 32;
	static const size_t commands = 8;
	Genome() : generation(0), hash(Hashing()), mutationChance((rand() % 21 + 10) / (double)100), index(0) {};
	explicit Genome(std::array<Gen, length> d, double mh, size_t g = 1)
		: generation(g), mutationChance(mh), data(d), hash(Hashing()), index(0) {};

	size_t generation;
	double mutationChance;
	std::array<Gen, length> data;

	RGBColor Hash()
	{
		return hash;
	}

	Gen::Command Read()
	{
		if (index + 1 == data.size())
		{
			index = 0;
			return data[data.size() - 1].Read();
		}
		else
			return data[index++].Read();
	}
private:
	RGBColor Hashing()
	{
		std::stringstream rs;
		unsigned char g = 0;
		unsigned char b = 0;
		for (unsigned short i = 0; i < Genome::length; i++)
		{
			unsigned char gen = static_cast<unsigned char>(data[i].Read())* ((double)255 / (Genome::length * Genome::commands));
			rs << gen;
			g += gen * (((double)255 / Genome::length) * Genome::commands);
			b += gen * ((double)255 / (Genome::length * Genome::commands));
		}

		unsigned char r = std::hash<std::string>{}(rs.str()) % 255;
		return { r, g, b };
	}
	const RGBColor hash;
	// указатель текущей команды
	size_t index = 0;
};
