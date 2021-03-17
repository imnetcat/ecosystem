#include "genome.h"

const std::uniform_int_distribution<unsigned __int64> Genome::distributor = std::uniform_int_distribution<unsigned __int64>(1, args_max - 1);

unsigned __int64 Genome::start_data[64];
std::mt19937_64 Genome::engine;
unsigned __int64 Genome::genome_seed;

Genome::Genome()
	: genome(0)
	, props(0)
	, args(0)
	, genome_size(0)
	, cursor(0)
	, generation(1)
	, replicate_cost(0)
	, mutationChance(0)
	, ration({ 0, 0, 0 })
	, species({ 0, 0, 0 })
{
	Construct();
}

// mutationChance must be from 0 to 100
Genome::Genome(
	unsigned __int64 genome,
	unsigned __int8 genome_size,
	unsigned __int8 args, 
	unsigned __int64 generation, 
	unsigned __int8 mutationChance
)
	: genome(genome)
	, genome_size(genome_size > 64 ? 64 : genome_size)
	, props(0)
	, args(args)
	, cursor(0)
	, generation(generation)
	, replicate_cost(0)
	, mutationChance(mutationChance)
	, ration({ 0, 0, 0 })
	, species({ 0, 0, 0 })
{
	Construct();
}

Gen Genome::Read()
{
	if (cursor == genome_size)
	{
		cursor = 0;
	}

	bool bit = (genome >> (genome_size - cursor - 1)) & 1;
	unsigned __int64 base = start_data[cursor];
	unsigned __int64 trigger = base;
	if (bit)
	{
		trigger *= genome_seed % base + genome_seed;
	}
	trigger %= trigger_max;

	Operation current_trigger = static_cast<Operation>(trigger);

	unsigned __int8 current_arg = base;
	bit = (args >> (args_size - cursor - 1)) & 1;
	if (bit)
	{
		current_arg *= genome_seed % current_arg + genome_seed;
	}
	current_arg %= args_max;

	cursor++;
	return { current_trigger, current_arg };
}
unsigned __int64 Genome::Data() const
{
	return genome;
}
unsigned __int8 Genome::Args() const
{
	return args;
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
	return genome_size;
}
const Color& Genome::Species() const
{
	return species;
}
const Color& Genome::Ration() const
{
	return ration;
}
unsigned __int16 Genome::ReplicateCost() const
{
	return replicate_cost;
}

void Genome::Init(unsigned __int64 genome_seed)
{
	Genome::genome_seed = genome_seed;
	Genome::engine = std::mt19937_64(Genome::genome_seed);
	for (int i = 0; i < 64; i++)
	{
		Genome::start_data[i] = Genome::distributor(Genome::engine);
	}
}
Genome Genome::Replicate(Coefficient coef)
{
	auto new_genom = genome;
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
			new_genom ^= (1ull << (rand % genome_size));
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
			new_genom |= (1ull << (rand % genome_size));
			break;
		case 7:
			// Write 1 to the bit in args
			new_args |= (1ull << (rand % genome_size));
			break;
		case 8:
			// Write 0 to the bit in genom
			new_genom &= ~(1ull << (rand % genome_size));
			break;
		case 9:
			// Write 0 to the bit in args
			new_args &= ~(1ull << (rand % genome_size));
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

	return { new_genom, genome_size, new_args, generation + 1, new_mutationChance };
}

void Genome::Construct()
{
	// Hash species
	unsigned char part1, part2, part3;
	if (genome_size % 3 == 0)
	{
		part1 = part2 = genome_size / 3;
	}
	else if (genome_size % 3 == 1)
	{
		part1 = part2 = genome_size / 3;
	}
	else
	{
		part1 = genome_size / 3;
		part2 = genome_size / 3 + 1;
	}
	part3 = genome_size - (part2 + part1);

	for (unsigned short i = 0; i < part1; i++)
	{
		species.r += static_cast<unsigned char>(Read().trigger);
	}
	for (unsigned short i = 0; i < part2; i++)
	{
		species.g += static_cast<unsigned char>(Read().trigger);
	}
	for (unsigned short i = 0; i < part3; i++)
	{
		species.b += static_cast<unsigned char>(Read().trigger);
	}

	for (unsigned short i = 0; i < part1; i++)
	{
		species.r += Read().args;
	}
	for (unsigned short i = 0; i < part2; i++)
	{
		species.g += Read().args;
	}
	for (unsigned short i = 0; i < part3; i++)
	{
		species.b += Read().args;
	}

	species.r %= 255;
	species.g %= 255;
	species.b %= 255;

	double food_triggers = 0;
	for (unsigned __int8 i = 0; i < genome_size; i++)
	{
		Gen gen = Read();

		// Calculate reproduction cost
		replicate_cost += CREATION_COST.at(gen.trigger);

		// Acummulate ration
		switch (gen.trigger)
		{
		case Operation::Carnivorous:
			food_triggers++;
			ration.r++;
			break;
		case Operation::Photosyntesis:
			food_triggers++;
			ration.g++;
			break;
		case Operation::EatOrganic:
			food_triggers++;
			ration.b++;
			break;
		}
	}

	// Hash ration
	ration.r = (ration.r / food_triggers) * 255;
	ration.g = (ration.g / food_triggers) * 255;
	ration.b = (ration.b / food_triggers) * 255;
}
