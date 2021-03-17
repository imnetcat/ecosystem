#pragma once
#include <map>
enum class Operation
{
	Stay,
	Move,
	Photosyntesis,
	EatOrganic,
	Carnivorous,
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
	{Operation::Photosyntesis, 10},
	{Operation::EatOrganic, 15},
	{Operation::Carnivorous, 25},
	{Operation::Birth, 30},
	{Operation::Separate, 30}
};

const std::map<Operation, unsigned int> MAINTENANACE_COST = {
	{Operation::Stay, 1},
	{Operation::Move, 5},
	{Operation::Photosyntesis, 1},
	{Operation::EatOrganic, 2},
	{Operation::Carnivorous, 5},
	{Operation::Birth, 6},
	{Operation::Separate, 6}
};