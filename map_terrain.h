#pragma once

#include <array>
#include <memory>
#include "config.h"
#include "structure.h"
#include "cell.h"

struct Position
{
	size_t x;
	size_t y;
};

bool operator == (const Position& lhs, const Position& rhs);

const int ENVIRONMENT_SIZE_X = 110;
const int ENVIRONMENT_SIZE_Y = 60;

Position GetViewedPosition(view_side view, Position init);

static size_t cells_count = 0;
static std::array<std::array<Structure, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
static std::array<Cell*, ENVIRONMENT_SIZE_X * ENVIRONMENT_SIZE_Y> cells;
