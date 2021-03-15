#include "Genome.h"

const std::uniform_int_distribution<unsigned __int64> Genome::distributor = std::uniform_int_distribution<unsigned __int64>(1, args_max - 1);

unsigned __int64 Genome::start_data[64];
std::mt19937_64 Genome::engine;
unsigned __int64 Genome::world_seed;

Genome::Genome()
	: genom(random.Generate(genome_max))
	, props(0)
	, args(random.Generate(args_max))
	, cursor(0)
	, generation(1)
	, replicate_cost(0)
	, mutationChance(10)
	, ration(Ration::omnivorous)
{
	Construct();
}

// mutationChance must be from 0 to 100
Genome::Genome(unsigned __int64 genom, unsigned __int8 args, unsigned __int64 generation, unsigned __int8 mutationChance)
	: genom(genom)
	, props(0)
	, args(args)
	, cursor(0)
	, generation(generation)
	, replicate_cost(0)
	, mutationChance(mutationChance)
	, ration(Ration::omnivorous)
{
	Construct();
}

Gen Genome::Read()
{
	if (cursor == genom_size)
	{
		cursor = 0;
	}

	bool bit = (genom >> (genom_size - cursor - 1)) & 1;
	unsigned __int64 base = start_data[cursor];
	unsigned __int64 trigger = base;
	if (bit)
	{
		trigger *= world_seed % base + world_seed;
	}
	trigger %= trigger_max;

	Trigger current_trigger = static_cast<Trigger>(trigger);

	unsigned __int8 current_arg = base;
	bit = (args >> (args_size - cursor - 1)) & 1;
	if (bit)
	{
		current_arg *= world_seed % current_arg + world_seed;
	}
	current_arg %= args_max;

	cursor++;
	return { current_trigger, current_arg };
}
unsigned __int64 Genome::Data() const
{
	return genom;
}
unsigned __int64 Genome::Generation() const
{
	return generation;
}
unsigned __int8 Genome::MutationChance() const
{
	return mutationChance;
}
inline unsigned __int8 Genome::Size() const
{
	return genom_size;
}
const RGBColor& Genome::Species() const
{
	return species;
}
Ration Genome::Ration() const
{
	return ration;
}
unsigned __int16 Genome::ReplicateCost() const
{
	return replicate_cost;
}

void Genome::Init(unsigned __int64 world_seed)
{
	Genome::world_seed = world_seed;
	Genome::engine = std::mt19937_64(Genome::world_seed);
	for (int i = 0; i < 64; i++)
	{
		Genome::start_data[i] = Genome::distributor(Genome::engine);
	}
}
Genome Genome::Replicate(Coefficient coef)
{
	auto new_genom = genom;
	auto new_args = args;

	unsigned __int8 rand = random.Generate(100);
	// Is mutation be
	if (rand < mutationChance)
	{
		// Define mutation type
		switch (rand % 10)
		{
		case 0:
			// Invert bit in genome
			new_genom ^= (1ull << (rand % genom_size));
			break;
		case 1:
			// Invert bit in args
			new_args ^= (1ull << (rand % args_size));
			break;
		case 2:
			// Left bitwise shift genome
			new_genom = new_genom << 1;
			break;
		case 3:
			// Right bitwise shift genome
			new_genom = new_genom >> 1;
			break;
		case 4:
			// Left bitwise shift args
			new_args = new_args << 1;
			break;
		case 5:
			// Right bitwise shift args
			new_args = new_args >> 1;
			break;
		case 6:
			// Write 1 to the bit in genom
			new_genom |= (1ull << (rand % genom_size));
			break;
		case 7:
			// Write 1 to the bit in args
			new_args |= (1ull << (rand % genom_size));
			break;
		case 8:
			// Write 0 to the bit in genom
			new_genom &= ~(1ull << (rand % genom_size));
			break;
		case 9:
			// Write 0 to the bit in args
			new_args &= ~(1ull << (rand % genom_size));
			break;
		default:
			break;
		}
	}

	unsigned __int8 new_mutationChance = mutationChance;

	switch (coef)
	{
	case Coefficient::enlarge:
		if (new_mutationChance > 0) new_mutationChance--;
		break;
	case Coefficient::reduce:
		if (new_mutationChance < 100) new_mutationChance++;
		break;
	case Coefficient::unchanged:
	default:
		break;
	}

	return { new_genom, new_args, generation + 1, new_mutationChance };
}

void Genome::Construct()
{
	bool Carnivorous = false;
	bool Photosyntesis = false;
	bool Organic = false;

	for (unsigned __int8 i = 0; i < genom_size; i++)
	{
		Gen gen = Read();

		// Calculate reproduction cost
		replicate_cost += CREATION_COST.at(gen.trigger);

		// Hash species
		unsigned __int8 trigger = static_cast<unsigned __int8>(gen.trigger);
		unsigned __int8 args = gen.args;
		species.r += std::hash<unsigned __int8>{}(trigger) % 128;
		species.g += std::hash<unsigned __int8>{}(trigger % trigger_max) % 128;
		species.b += std::hash<unsigned __int8>{}(trigger ^ (trigger_max * trigger_max)) % 128;

		species.r += std::hash<unsigned __int8>{}(args) % 128;
		species.g += std::hash<unsigned __int8>{}(args % args_max) % 128;
		species.b += std::hash<unsigned __int8>{}(args ^ (args_max * args_max)) % 128;

		// Acummulate ration
		switch (gen.trigger)
		{
		case Trigger::Carnivorous:
			Carnivorous = true;
			break;
		case Trigger::Photosyntesis:
			Photosyntesis = true;
			break;
		case Trigger::Mineraleon:
			Organic = true;
			break;
		}
	}

	ration = Ration::omnivorous;
	// Define ration
	if (Carnivorous && Photosyntesis)
	{
		ration = Ration::entites_light;
	}
	else if (Carnivorous && Organic)
	{
		ration = Ration::entites_organic;
	}
	else if (Photosyntesis && Organic)
	{
		ration = Ration::light_organic;
	}
	else if (Photosyntesis)
	{
		ration = Ration::light;
	}
	else if (Organic)
	{
		ration = Ration::organic;
	}
	else if (Carnivorous)
	{
		ration = Ration::entities;
	}
}