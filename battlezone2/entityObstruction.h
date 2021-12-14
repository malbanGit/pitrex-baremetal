#pragma once

#include "entity.h"

class entityObstruction :
	public entity
{
public:
	entityObstruction();
	~entityObstruction();

	BOOL mObstructsFire;
	BOOL mObstructsMovement;
};

