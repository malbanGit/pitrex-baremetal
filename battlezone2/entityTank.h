#pragma once

#include "enemy.h"

class entityTank :
	public enemy
{
public:

	entityTank();
	~entityTank();

    virtual void run();
    virtual void spawnTransition();

    virtual void draw(const camera& cam);

protected:
	void runBehavior();
	virtual void alignParts();

	float mRotationForeAndAft;
	float mRotationSideToSide;

	model* mRadarModel;
	model* mRadarPostModel;

	float mRadarHeading;

	float mTurnFactor;
	float mSpeedFactor;

	int mInitialFiringTimer;

};

