#pragma once

#include <array>
#include <memory>
#include "config.h"

class Structure;

struct Position
{
	size_t x;
	size_t y;
};

bool operator == (const Position& lhs, const Position& rhs);

const int ENVIRONMENT_SIZE_X = 110;
const int ENVIRONMENT_SIZE_Y = 60;

enum view_side
{
	top,
	right_top,
	right,
	right_bottom,
	bottom,
	left_bottom,
	left,
	left_top,
};

Position GetViewedPosition(view_side view, Position init);

using MapTerrain = std::array<std::array<std::shared_ptr<Structure>, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y>;
