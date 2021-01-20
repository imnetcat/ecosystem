#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include <vector>
#include "object.h"
#include "Gen.h"

class Genome
{
public:

	explicit Genome(std::vector<Gen> d, float mh, size_t g)
		: generation(g), mutationChance(mh), data(d), hash(Hashing()), index(0) {};

	explicit Genome()
		: generation(1), mutationChance((rand() % 100) / float(100)), index(0)
	{
		auto size = static_cast<int>(Trigger::Count);
		data.resize(size);
		for (int i = 0; i < size; i++)
		{
			data[i].trigger = static_cast<Trigger>(rand() % size);
			data[i].args = rand() % 256;
		}
		hash = Hashing();
	};

	Genome& operator = (const Genome& genome)
	{
		generation = genome.generation;
		mutationChance = genome.mutationChance;
		data = genome.data;
		index = genome.index;
		return *this;
	}

	size_t generation;
	float mutationChance;
	std::vector<Gen> data;

	RGBColor Hash()
	{
		return hash;
	}

	Gen Read()
	{
		Gen gen;
		if ((index + 1) == data.size())
		{
			index = 0;
			gen = data[data.size() - 1];
		}
		else
			gen = data[index++];

		return gen;
	}

	Genome Replicate(float mutationChance_koef)
	{
		srand(time(0) + rand());
		auto new_genom = data;
		float mt = (rand() % 100) / (float)100;
		// is mutation be
		if (mt < mutationChance)
		{
			// common mutation
			// one of gen in genome changed
			size_t index = rand() % new_genom.size();
			new_genom[index].trigger = static_cast<Trigger>(rand() % static_cast<int>(Trigger::Count));
			new_genom[index].args = rand() % 256;

			// rare mutation
			// add or remove gen from genome
			mt = (rand() % 100) / (float)100;
			if (mt < (mutationChance / 2))
			{
				int t = mt * 100;
				if (t % 2)
				{
					size_t index = rand() % new_genom.size();
					new_genom.erase(new_genom.begin() + index);
				}
				else
				{
					Gen gen;
					gen.trigger = static_cast<Trigger>(rand() % static_cast<int>(Trigger::Count));
					gen.args = rand() % 256;
					new_genom.push_back(gen);
				}
			}
		}

		float new_mutationChance = mutationChance + mutationChance_koef;
		if (new_mutationChance > 1) new_mutationChance = 1;
		if (new_mutationChance < 0) new_mutationChance = 0;

		return Genome(new_genom, new_mutationChance, generation + 1);
	}
private:
	RGBColor Hashing()
	{
		std::stringstream rs;
		std::stringstream gs;
		std::stringstream bs;
		for (const auto& g : data)
		{
			auto gen = static_cast<unsigned int>(g.trigger);
			rs << (gen * data.size()) % 255;
			gs << (gen % data.size()) % 255;
			bs << ((gen ^ (gen * gen)) % data.size()) % 255;
		}
		for (const auto& g : data)
		{
			auto gen = static_cast<unsigned int>(g.args);
			rs << (gen * data.size()) % 255;
			gs << (gen % data.size()) % 255;
			bs << ((gen ^ (gen * gen)) % data.size()) % 255;
		}
		unsigned char r = std::hash<std::string>{}(rs.str()) % 255;
		unsigned char g = std::hash<std::string>{}(gs.str()) % 255;
		unsigned char b = std::hash<std::string>{}(bs.str()) % 255;
		return { r, g, b };
	}
	RGBColor hash;

	size_t index = 0;
};
