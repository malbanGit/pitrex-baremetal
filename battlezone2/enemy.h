#pragma once

#include "entity.h"

class enemy :
	public entity
{
public:
	enemy();
	~enemy();

	enum AIState
	{
		AISTATE_THINKING=0,
		AISTATE_AIMING_LEFT,
		AISTATE_AIMING_RIGHT,
		AISTATE_APPROACHING,
		AISTATE_RETREATING,
		AISTATE_OBSTRUCTIONAVOID_LEFT,
		AISTATE_OBSTRUCTIONAVOID_RIGHT,
		AISTATE_OBSTRUCTIONAVOID_FORWARD,
		AISTATE_FIRING
	};

	int mPointValue;

protected:
	virtual void runBehavior()=0;
	virtual void alignParts() {;}

	AIState mAIState;
	int mAIThinkingTimer;
	int mAIStateTimer;

	int mReloadTimer;
	int mReloadTime;

	float mRotationForeAndAft;
};

