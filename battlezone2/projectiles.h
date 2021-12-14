#pragma once

#include "camera.h"
#include "entityProjectile.h"

#define NUM_PROJECTILES	10

class projectiles
{
public:
	projectiles(void);
	~projectiles(void);

	void run();
	void draw(const camera& cam);

	entityProjectile* getUnusedProjectile();

private:
    entityProjectile* mProjectiles[NUM_PROJECTILES];
};

