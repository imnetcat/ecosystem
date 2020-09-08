#pragma once
#include <array>
#include <ctime>

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
	Gen() : generation(0), mutationChance((rand() % 100) / (double)100) 
	{
		srand(time(0));
		for (size_t i = 0; i < length; i++)
		{
			data[i] = static_cast<Command>(rand() % Gen::commands);
		}
	};
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
