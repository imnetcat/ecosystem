#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include <map>
#include <vector>
#include "Gen.h"

struct RGBColor
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
};

enum class Ration
{
	cells,
	light,
	minerals
};

class Genome
{
public:

	explicit Genome(std::vector<Gen> d, double mh, size_t g)
		: generation(g), mutationChance(mh), data(d), index(0) 
	{
		hash = Hashing();
		RationHashing();
	}

	explicit Genome()
		: generation(1), mutationChance(0.0), index(0)
	{
		auto size = static_cast<int>(Trigger::Count);
		data.resize(size);
		for (int i = 0; i < size; i++)
		{
			data[i].trigger = static_cast<Trigger>(rand() % size);
			data[i].args = rand() % 256;
		}
		hash = Hashing();
		RationHashing();
	};

	Genome& operator = (const Genome& genome)
	{
		generation = genome.generation;
		mutationChance = genome.mutationChance;
		data = genome.data;
		hash = genome.hash;
		ration_map = genome.ration_map;
		index = genome.index;
		return *this;
	}

	size_t generation;
	double mutationChance;
	std::vector<Gen> data;

	const RGBColor& Hash() const
	{
		return hash;
	}
	const std::map<Ration, bool>& RationMap() const
	{
		return ration_map;
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

	Genome Replicate(double mutationChance_koef)
	{
		auto new_genom = data;
		double mt = (rand() % 100) / (double)100;
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
			mt = (rand() % 100) / (double)100;
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

		double new_mutationChance = mutationChance + mutationChance_koef;
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
			//auto gen = static_cast<unsigned int>(g.args);
			//rs << (gen * data.size()) % 255;
			//gs << (gen % data.size()) % 255;
			//bs << ((gen ^ (gen * gen)) % data.size()) % 255;
		}
		unsigned char r = std::hash<std::string>{}(rs.str()) % 255;
		unsigned char g = std::hash<std::string>{}(gs.str()) % 255;
		unsigned char b = std::hash<std::string>{}(bs.str()) % 255;
		return { r, g, b };
	}
	void RationHashing()
	{
		ration_map[Ration::cells] = false;
		ration_map[Ration::light] = false;
		ration_map[Ration::minerals] = false;
		for (const auto& g : data)
		{
			switch (g.trigger)
			{
			case Trigger::Carnivorous:
				ration_map[Ration::cells] = true;
				break;
			case Trigger::Photosyntesis:
				ration_map[Ration::light] = true;
				break;
			case Trigger::Mineraleon:
				ration_map[Ration::minerals] = true;
				break;
			}
			if (ration_map[Ration::cells] && ration_map[Ration::light] && ration_map[Ration::minerals])
				break;
		}
	}
	RGBColor hash;
	std::map<Ration, bool> ration_map;

	size_t index = 0;
};
