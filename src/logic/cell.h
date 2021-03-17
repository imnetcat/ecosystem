#pragma once
#include "Organic.h"
#include "Entity.h"

class cell
{
protected:
	OrganicIterator object;
	EntitiesIterator entity;
	bool has_entity = false;
	bool has_object = false;
public:

	void SetEntity(EntitiesIterator);
	void DelEntity();
	EntitiesIterator GetEntity() const;
	bool ContainsEntity() const;

	void SetOrganic(OrganicIterator organic);
	void AddOrganic(size_t value);
	void DelOrganic();
	OrganicIterator GetOrganic() const;
	bool IsContainsOrganic() const;
};
