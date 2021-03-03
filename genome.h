#pragma once
#include <array>
#include <ctime>
#include <sstream>
#include <map>
#include <vector>
#include "Gen.h"
#include "Random.h"

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
private:
	Random random; 
	std::map<Ration, bool> ration_map;
	size_t index = 0;
	RGBColor hash;
public:

	size_t generation;
	double mutationChance;
	std::vector<Gen> data;

	Genome() : generation(1), mutationChance(0.0), index(0){};

	Genome(std::vector<Gen> data, double mutationChance, size_t generation)
		: generation(generation), mutationChance(mutationChance), data(data), index(0)
	{
		hash = Hashing();
		RationHashing();
	}

	Genome(const Genome& obj)
		: generation(obj.generation)
		, mutationChance(obj.mutationChance)
		, index(obj.index)
		, hash(obj.hash)
		, data(obj.data)
		, ration_map(obj.ration_map) {}

	Genome(Genome&&) = default;
	Genome& operator = (const Genome& obj)
	{
		generation = obj.generation;
		mutationChance = obj.mutationChance;
		index = obj.index;
		hash = obj.hash;
		data = obj.data;
		ration_map = obj.ration_map;
		return *this;
	}
	Genome& operator = (Genome&&) = default;

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
		// is mutation be
		if (random.Chance(mutationChance))
		{
			// common mutation
			// one of gen in genome changed
			size_t index = random.Generate(new_genom.size());
			new_genom[index].trigger = static_cast<Trigger>(random.Generate(static_cast<int>(Trigger::Count)));
			new_genom[index].args = random.Generate(256);

			// rare mutation
			// add or remove gen from genome
			if (random.Chance(mutationChance / 2))
			{
				if (random.Generate(2))
				{
					size_t index = random.Generate(new_genom.size());
					new_genom.erase(new_genom.begin() + index);
				}
				else
				{
					Gen gen;
					gen.trigger = static_cast<Trigger>(random.Generate(static_cast<int>(Trigger::Count)));
					gen.args = random.Generate(256);
					new_genom.push_back(gen);
				}
			}
		}

		double new_mutationChance = mutationChance + mutationChance_koef;
		if (new_mutationChance > 1) new_mutationChance = 1;
		if (new_mutationChance < 0) new_mutationChance = 0;

		return { new_genom, new_mutationChance, generation + 1 };
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
};
