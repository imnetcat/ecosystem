#pragma once
#include <array>

enum Command
{
	die,
	stay,
	move,
	turn_left,
	turn_right,
	fotosintesis,
	reproduction,  // деление на две
};

class Gen
{
public:
	static const size_t length = 32;
	static const size_t commands = 6;
	explicit Gen(std::array<Command, length> d, double mh, size_t g = 1)
		: generation(g), mutationChance(mh), data(d) {};

	size_t generation;
	double mutationChance;
	std::array<Command, length> data;

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

	// указатель текущей команды
	size_t index = 0;
};
