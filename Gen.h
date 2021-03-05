#pragma once
#include <map>
enum class Trigger
{
	Stay,
	Move,
	Turn,
	Photosyntesis,
	Mineraleon,
	Carnivorous,
	Birth,
	Separate,

	Count
};

struct Gen
{
	Trigger trigger;
	unsigned __int8 args;
};

const std::map<Trigger, unsigned int> CREATION_COST = {
	{Trigger::Stay, 1},
	{Trigger::Move, 15},
	{Trigger::Turn, 2},
	{Trigger::Photosyntesis, 10},
	{Trigger::Mineraleon, 15},
	{Trigger::Carnivorous, 25},
	{Trigger::Birth, 30},
	{Trigger::Separate, 30}
};

const std::map<Trigger, unsigned int> MAINTENANACE_COST = {
	{Trigger::Stay, 1},
	{Trigger::Move, 5},
	{Trigger::Turn, 1},
	{Trigger::Photosyntesis, 1},
	{Trigger::Mineraleon, 2},
	{Trigger::Carnivorous, 5},
	{Trigger::Birth, 6},
	{Trigger::Separate, 6}
};