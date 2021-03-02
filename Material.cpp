#include "Material.h"
#include "config.h"

Material::Material(RGBColor color, bool walkable, float transparency) :
	color(color),
	walkable(walkable),
	transparency(transparency)
{ }

bool Material::IsWalkable()
{
	return walkable;
}
