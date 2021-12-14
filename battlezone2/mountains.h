#pragma once

#include "particle2d.h"

class mountains
{
public:
	mountains(void);
	~mountains(void);

	void run();
	void draw(float heading);

    particle2d mParticles2d;

private:

	float mMountainHeading;

};

