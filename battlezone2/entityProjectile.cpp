#include "entityProjectile.h"
#include "objloader.h"

entityProjectile::entityProjectile()
{
	mType = ENTITY_TYPE_PROJECTILE;

	mModel = objloader::loadFile((char*)"models/projectile.obj");
	model::flipXY(mModel);

	mScale = 1;
	mModel->Scale(1);

    mSpawnTime = 0;
    mDestroyTime = 20;

	mDurationTimer = 0;

	mPen = vector::penGreen();
}

entityProjectile::~entityProjectile()
{
}
