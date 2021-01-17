#include "map_terrain.h"

bool operator == (const Position& lhs, const Position& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

Position GetViewedPosition(view_side view, Position init)
{
	auto maxX = ENVIRONMENT_SIZE_X - 1;
	auto maxY = ENVIRONMENT_SIZE_Y - 1;

	Position viewed_position = init;
	switch (view)
	{
	case view_side::left:
		if (init.x > 0)
		{
			viewed_position.x--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
		}
		break;
	case view_side::right:
		if (init.x < maxX)
		{
			viewed_position.x++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
		}
		break;
	case view_side::bottom:
		if (init.y > 0)
		{
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.y = maxY;
		}
		break;
	case view_side::top:
		if (init.y < maxY)
		{
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.y = 0;
		}
		break;
	case view_side::left_bottom:
		if (init.x > 0 && init.y < maxY)
		{
			viewed_position.x--;
			viewed_position.y++;
		}
		else if (init.y < maxY && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT && init.x > 0)
		{
			viewed_position.x--;
			viewed_position.y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y = 0;
		}
		break;
	case view_side::left_top:
		if (init.x > 0 && init.y > 0)
		{
			viewed_position.x--;
			viewed_position.y--;
		}
		else if (init.y > 0 && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT && init.x > 0)
		{
			viewed_position.x--;
			viewed_position.y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = maxX;
			viewed_position.y = maxY;
		}
		break;
	case view_side::right_bottom:
		if (init.y < maxY && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y++;
		}
		else if (init.y < maxY && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y++;
		}
		else if (LOOPED_ENVIRONMENT && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y = 0;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y = 0;
		}
		break;
	case view_side::right_top:
		if (init.y > 0 && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y--;
		}
		else if (init.y > 0 && LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y--;
		}
		else if (LOOPED_ENVIRONMENT && init.x < maxX)
		{
			viewed_position.x++;
			viewed_position.y = maxY;
		}
		else if (LOOPED_ENVIRONMENT)
		{
			viewed_position.x = 0;
			viewed_position.y = maxY;
		}
		break;
	}
	return viewed_position;
}
