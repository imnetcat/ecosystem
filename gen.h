#pragma once
#include <array>

class Gen
{
public:
	static const size_t length = 16;
	static const size_t commands = 6;
	explicit Gen(std::array<Command, length> d, size_t mh, size_t g = 1)
		: generation(g), mutationChance(mh), data(d) {};

	size_t generation;
	size_t mutationChance;
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

	// ��������� ������� �������
	size_t index = 0;
};
