#pragma once
#include "ration.h"
#include "object.h"
#include "gen.h"

// абстрактное живое существо
class Entity : public Object
{
public:
	explicit Entity(Ration ph, Gen g)
		: ration(ph), genom(g)  {	}

	virtual Ration GetRation() const = 0;
	virtual unsigned short Die() const = 0;
	virtual bool Dying() const = 0;
	virtual void Fotosintesis(unsigned short value) = 0;

	ObjectColor Color() override
	{
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		for (unsigned short i = ration.entities; i > 1; i--)
		{
			if(r != 250)
				r += 50;

			g = 0;
			b = 0;
		}
		for (unsigned short i = ration.fotosintesis; i > 1; i--)
		{
			if (g == 250 && r != 0)
				r -= 50;
			if (g != 250)
				g += 50;
			b = 0;
		}
		for (unsigned short i = ration.minerals; i > 1; i--)
		{
			if (r != 250)
				r += 50;
			if (g != 250)
				g += 50;
			b = 0;
		}
		return ObjectColor{ r, g, b };
	}
protected:
	Ration ration;
	Gen genom;
};
