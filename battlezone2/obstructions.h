#pragma once

#include "entity.h"

class obstructions
{
public:
	obstructions();
	~obstructions();

	void run();
	void draw(const camera& cam);

	entity* hitTestObstructionsAtPoint(Point3d point, float radius=0);

	entity** mObstructions;
};

