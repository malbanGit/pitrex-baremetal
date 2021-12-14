#pragma once

#include "particle.h"

class particle2d : public particle
{
public:
	particle2d();
	~particle2d();

	virtual void run();
	virtual void draw(float heading, const matrix& mat);
	virtual void emitter(Point3d* position, Point3d* angle, float speed, float spread, int num, vector::pen* color, int timeToLive, BOOL gravity=TRUE, float drag=.93, BOOL groundBounce=TRUE);

};

