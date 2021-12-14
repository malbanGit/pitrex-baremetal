#pragma once

#include "enemy.h"

class entitySaucer :
	public enemy
{
public:
	entitySaucer();
	~entitySaucer();

    virtual void run();
    virtual void spawnTransition();
    virtual void destroyTransition();
    virtual void destroy();

    virtual void draw(const camera& cam);

private:
	void runBehavior();

	float mAngleFromPlayer;
	int mChangeDirTimer;
	int mMoveDir;

};

