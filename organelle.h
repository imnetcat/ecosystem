#pragma once
#include "map_terrain.h"
#include "protein.h"
#include <unordered_map>
class Organelle
{
public:
	virtual void Event(MapTerrain& terrain, size_t& x, size_t& y) const = 0;
	virtual bool HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const = 0;
	virtual bool IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const = 0;
};
