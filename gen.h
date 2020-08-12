#pragma once
#include <array>

class Gen
{
public:
	explicit Gen(std::array<Command, 8> d) : data(d) {};
	std::array<Command, 8> data;

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
