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
	int args;
};

const std::map<Trigger, unsigned int> CREATION_COST = {
	{Trigger::Stay, 1},
	{Trigger::Move, 15},
	{Trigger::Turn, 10},
	{Trigger::Photosyntesis, 15},
	{Trigger::Mineraleon, 15},
	{Trigger::Carnivorous, 25},
	{Trigger::Birth, 30},
	{Trigger::Separate, 40}
};

const std::map<Trigger, unsigned int> MAINTENANACE_COST = {
	{Trigger::Stay, 1},
	{Trigger::Move, 5},
	{Trigger::Turn, 2},
	{Trigger::Photosyntesis, 5},
	{Trigger::Mineraleon, 5},
	{Trigger::Carnivorous, 10},
	{Trigger::Birth, 12},
	{Trigger::Separate, 15}
};