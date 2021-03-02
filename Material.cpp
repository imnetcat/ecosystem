#include "Material.h"
#include "config.h"

Material::Material(RGBColor color, bool walkable, float transparency, GravityEffect gravity) :
	Object(color, walkable, transparency, gravity) { }
