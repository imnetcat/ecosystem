#pragma once
#include <map>
namespace Ecosystem
{
	namespace Logic
	{
		enum class Operation
		{
			Stay,
			Move,
			Photosyntesis,
			EatOrganic,
			Attack,
			Birth,
			Separate,

			Count
		};

		/*
		*	Gen structure:
		*
		*	Operation - defines the operation that the gen encodes
		*
		*	args - number, every bit of which defines arguments for
		*	 operation accordingly, for each operation, there can
		*	 be as many arguments as there are bits in the number
		*/
		struct Gen
		{
			Operation trigger;
			unsigned __int8 args;
		};

		const std::map<Operation, unsigned int> CREATION_COST = {
			{Operation::Stay, 1},
			{Operation::Move, 15},
			{Operation::Photosyntesis, 20},
			{Operation::EatOrganic, 40},
			{Operation::Attack, 60},
			{Operation::Birth, 100},
			{Operation::Separate, 100}
		};

		const std::map<Operation, unsigned int> MAINTENANACE_COST = {
			{Operation::Stay, 1},
			{Operation::Move, 2},
			{Operation::Photosyntesis, 2},
			{Operation::EatOrganic, 4},
			{Operation::Attack, 8},
			{Operation::Birth, 10},
			{Operation::Separate, 10}
		};
	}
}