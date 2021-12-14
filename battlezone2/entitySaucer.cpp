#include "entitySaucer.h"
#include "game.h"
#include "objloader.h"

entitySaucer::entitySaucer()
	: enemy()
{
	mType = ENTITY_TYPE_SAUCER;

	mPointValue = 5000;

	mModel = objloader::loadFile((char*)"models/saucer.obj");
	model::flipXY(mModel);

	mScale = 5;
	mRadius = 20;
	mModel->Scale(5);

	mSpawnTime = 500;
	mDestroyTime = 300;

	mPen = vector::penGreen();
	mPen.a *= 1.2;

	mPos = Point3d(0,-20,0);

	setState(entity::ENTITY_STATE_INACTIVE);
}

entitySaucer::~entitySaucer()
{
}

void entitySaucer::run()
{
	runBehavior();

	float playerHeading = game::mPlayers.mPlayer1.getHeading() + mathutils::DegreesToRads(90);
	float targetAngle = mathutils::calculate2dAngle(mPos, game::mPlayers.mPlayer1.getPos());
	float diff = mathutils::diffAngles(playerHeading, targetAngle);
	if (fabs((double)diff) < .5)
	{
		if (!game::mSound.isTrackPlaying(SOUNDID_SAUCER_LOOP))
			game::mSound.playTrack(SOUNDID_SAUCER_LOOP);
	}
	else
	{
		if (game::mSound.isTrackPlaying(SOUNDID_SAUCER_LOOP))
			game::mSound.stopTrack(SOUNDID_SAUCER_LOOP);
	}
}

void entitySaucer::spawnTransition()
{
	entity::spawnTransition();

	Point3d playerPos = game::mPlayers.mPlayer1.getPos();
	float distance = 400;
	mAngleFromPlayer = mathutils::frandFrom0To1() * (2*PI);
	Point3d spawnVector(0,0,distance);
	spawnVector = mathutils::rotate2dPoint(spawnVector, mAngleFromPlayer);

	mHeading = mathutils::frandFrom0To1() * (2*PI);

	mPos = playerPos + spawnVector;
	mPos.y = -6;

	mMoveDir = mathutils::frandFrom0To1() * 10 < 4 ? -1 : 1;

	mPen = vector::penGreen();
	mPen.a *= 1.2;

	if (mModel)
	{
		mModel->Identity();
		mModel->Scale(mScale);
		mModel->Rotate(-mRotation.x, -mHeading, -mRotation.z);
		mModel->Translate(mPos.inverse());
	}
}

void entitySaucer::destroyTransition()
{
	entity::destroyTransition();

	game::mSound.stopTrack(SOUNDID_SAUCER_LOOP);
	game::mSound.playTrack(SOUNDID_SAUCER_HIT);
}

void entitySaucer::destroy()
{
	entity::destroy();

	mPen = vector::penGreen();
	mPen.a = (mStateTimer / (float)mDestroyTime)-.5;
}

void entitySaucer::draw(const camera& cam)
{
	if ((getState() == entity::ENTITY_STATE_RUNNING)
		|| (getState() == entity::ENTITY_STATE_DESTROY_TRANSITION)
		|| (getState() == entity::ENTITY_STATE_DESTROYED))
	{
		entity::draw(cam);
	}
}

void entitySaucer::runBehavior()
{
	mHeading += .03;
	mAngleFromPlayer += .0006 * mMoveDir;

	if (--mChangeDirTimer < 0)
	{
		mChangeDirTimer = 150;
		if (mathutils::frandFrom0To1() * 100 < 50)
			mMoveDir = -mMoveDir;
	}

	float d = mathutils::calculate2dDistance(mPos, game::mPlayers.mPlayer1.getPos());

	if (d < 250)
	{
		// Run away (but slowly)
		d += .6;
	}
	else if (d > 600)
	{
		// Too far away - respawn it later
		mState = entity::ENTITY_STATE_INACTIVE;
		game::mSound.stopTrack(SOUNDID_SAUCER_LOOP);
	}

	Point3d playerPos = game::mPlayers.mPlayer1.getPos();
	float distance = d;
	Point3d spawnVector(0,0,distance);
	spawnVector = mathutils::rotate2dPoint(spawnVector, mAngleFromPlayer);

	mPos = playerPos + spawnVector;
	mPos.y = -6;

}


