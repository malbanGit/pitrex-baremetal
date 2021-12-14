#pragma once

#include "enemy.h"

class entityMissile :
	public enemy
{
public:
	entityMissile();
	~entityMissile();

    virtual void run();
    virtual void spawnTransition();
    virtual void destroyTransition();

    virtual entity* hitTest(const Point3d& pos, float radius=0);

    virtual void draw(const camera& cam);

    int mPatternIndex;

protected:
	void runBehavior();
	void setAltitude(float alt);

	float mAngleFromPlayer;

	float mCurrentAltitude;
	float mTargetAltitude;

	int mTargetTurnDirection;
	float mCurrentTurnDirection;

	int mTurnTimer;
    int mTurnIndex;
	BOOL mOvershoot;
};

