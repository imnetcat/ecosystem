#pragma once
#include <array>

class Gen
{
public:
	explicit Gen(std::array<unsigned short, 8> d) : data(d) {};
	std::array<unsigned short, 8> data;
};
