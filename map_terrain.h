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

Position GetViewedPosition(view_side view, Position init);
Position GetInvertedViewedPosition(view_side view, Position init);

static size_t cells_count = 0;
static size_t max_generation = 0;
static std::array<std::array<Structure, ENVIRONMENT_SIZE_X>, ENVIRONMENT_SIZE_Y> terrain;
static std::array<Cell*, ENVIRONMENT_SIZE_X * ENVIRONMENT_SIZE_Y> cells;

using Organelle = void (*)(int args, size_t x, size_t y);

static void Shift(size_t i)
{
	size_t index;
	for (index = i + 1; index < cells.size(); index++)
	{
		cells[index - 1] = cells[index];
		if (!cells[index]) break;
	}
	if (cells_count)
	{
		cells_count--;
	}
}

static void CellDie(size_t index, size_t x, size_t y)
{
	if (terrain[y][x].IsContainsFood())
		terrain[y][x].GetFood().Put(cells[index]->Energy() + 100);
	else
		terrain[y][x].SetFood(cells[index]->Energy() + 100);

	terrain[y][x].DelCell();
	Shift(index);
}
static void CellDie(size_t x, size_t y)
{
	size_t index;
	for (index = 0; index < cells.size(); index++)
	{
		if (cells[index]->GetX() == x &&
			cells[index]->GetY() == y)
			break;
	}

	if (index == cells.size()) return;

	if (terrain[y][x].IsContainsFood())
		terrain[y][x].GetFood().Put(cells[index]->Energy() + 100);
	else
		terrain[y][x].SetFood(cells[index]->Energy() + 100);

	terrain[y][x].DelCell();
	Shift(index);
}