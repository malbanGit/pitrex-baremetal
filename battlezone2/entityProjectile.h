#pragma once

#include "entity.h"

class entityProjectile :
	public entity
{
public:
	entityProjectile();
	~entityProjectile();

	int mDurationTimer;

	/*typedef*/ enum
	{
		PROJECTILE_SOURCE_ENEMY=0,
		PROJECTILE_SOURCE_PLAYER1
	};

	int mSource;
};

