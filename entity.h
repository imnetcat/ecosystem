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

	ObjectColor Color() override
	{
		switch (ration)
		{
		case Ration::minerals:
			return ObjectColor{ 173, 171, 0 }; // желтый
		case Ration::entities:
			return ObjectColor{ 255, 8, 0 }; // красный
		case Ration::fotosintesis:
			return ObjectColor{ 0, 156, 49 }; // зелёный
		}

		return ObjectColor{ 0, 0, 0 };
	}
protected:
	Ration ration;
	Gen genom;
};
