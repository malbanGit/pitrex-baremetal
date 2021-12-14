#pragma once

#include "camera.h"
#include "entityWreckage.h"

#define MAX_WRECKAGE 20

class wreckage
{
public:
	wreckage();
	~wreckage();

	void run();
	void draw(const camera& cam);

	void explodeTank(const entity& e, const Point3d& pos, float heading);

private:

	entityWreckage* mWreckage[MAX_WRECKAGE];
};

