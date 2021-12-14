#include "entitySupertank.h"
#include "objloader.h"


entitySupertank::entitySupertank()
{
	mType = ENTITY_TYPE_SUPERTANK;

	mPointValue = 3000;

	mModel = objloader::loadFile((char*)"models/supertank.obj");
	model::flipXY(mModel);

	mScale = 5;
	mRadius = 8;
	mModel->Scale(5);

	mSpawnTime = 70;
	mDestroyTime = 150;
	mReloadTime = 100;

	mTurnFactor = .01;
	mSpeedFactor = .6;

	mPen = vector::penGreen();
	mPen.a *= 1.2;

	mRadarHeading = 0;

	mAIState = AISTATE_THINKING;

	setState(entity::ENTITY_STATE_INACTIVE);
}

entitySupertank::~entitySupertank()
{
}

void entitySupertank::draw(const camera& cam)
{
	if (getState() == entity::ENTITY_STATE_RUNNING)
	{
		entity::draw(cam);
	}
}

