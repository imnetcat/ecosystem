#pragma once
#include "organelle.h"
#include <map>
class Turning : public Organelle
{
public:
	void Event(MapTerrain& terrain, size_t& x, size_t& y) const override;
	bool HaveRequestProteins(const std::unordered_map<Protein, unsigned long>& proteins) const override;
	bool IsNeeded(const std::unordered_map<Protein, unsigned long>& proteins) const override;
};