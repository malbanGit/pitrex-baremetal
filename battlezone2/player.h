#pragma once

#include "entity.h"
#include "camera.h"
#include "entityProjectile.h"

class player :
	public entity
{
public:
	player();
	~player();

	void startGame();

	void run();
	virtual void spawnTransition();
	virtual void destroyTransition();
	virtual void destroy();

	void draw();

	bool isTargetingEnemy();
	int getTargetDirection();

	void addPoints(int points);

	camera mCamera;

	float mForwardSpeed;
	float mRotationForeAndAft;
	float mRotationSideToSide;

	float mHeadingDiff;
	float mSpeedDiff;

	BOOL mMotionBlocked;
	BOOL mReloading;

	int mPoints;
	int mLives;

private:
	void runDigitalSteering(int leftStick, int rightStick);
	void drawWindowCracks();

	entityProjectile* mProjectile;

	float mCrackDistance;
	BOOL mHitSomething;

	float mCurrentEngineSpeed;
	float mTargetEngineSpeed;

	int mAttractModeTurnTimer;
	int mAttractModeTurnType;
};

