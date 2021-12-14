#include "entityTank.h"
#include "objloader.h"
#include "game.h"
#include "entityObstruction.h"

entityTank::entityTank()
	: enemy()
{
	mType = ENTITY_TYPE_TANK;

	mPointValue = 1000;

	mModel = objloader::loadFile((char*)"models/tank.obj");
	model::flipXY(mModel);

	mRadarModel = objloader::loadFile((char*)"models/tank_radar.obj");
	model::flipXY(mRadarModel);

	mRadarPostModel = objloader::loadFile((char*)"models/tank_radar_post.obj");
	model::flipXY(mRadarPostModel);

	mScale = 5;
	mRadius = 5;
	mModel->Scale(5);

	mSpawnTime = 70;
	mDestroyTime = 150;
	mReloadTime = 170;

	mTurnFactor = .007;
	mSpeedFactor = .3;

	mPen = vector::penGreen();
	mPen.a *= 1.2;

	mRadarHeading = 0;

	mAIState = AISTATE_THINKING;

	setState(entity::ENTITY_STATE_INACTIVE);
}

entityTank::~entityTank()
{
}

void entityTank::run()
{
	if (getEnabled() && (game::mGameMode == game::GAMEMODE_PLAYING))
	{
		runBehavior();
	}

	entity::run();

	mRadarHeading += .05;
	alignParts();
}

void entityTank::draw(const camera& cam)
{
	if (getState() == entity::ENTITY_STATE_RUNNING)
	{
		alignParts();

		entity::draw(cam);

		vector::pen pen(mPen);
		pen.a *= .5;
		mRadarModel->draw(cam, pen);
		mRadarPostModel->draw(cam, pen);
	}
}

void entityTank::alignParts()
{
	mRadarHeading = fmod((double)mRadarHeading, 2*PI);

	mRadarModel->Identity();
	mRadarModel->Scale(mScale);
	mRadarModel->Rotate(0, mRadarHeading, 0);
	mRadarModel->Translate(Point3d(-5.8,7.6,0));
	mRadarModel->Rotate(0, -mHeading, 0);
	mRadarModel->Translate(mPos.inverse());

	mRadarPostModel->Identity();
	mRadarPostModel->Scale(mScale);
	mRadarPostModel->Translate(Point3d(-5.8,7.6,0));
	mRadarPostModel->Rotate(0, -mHeading, 0);
	mRadarPostModel->Translate(mPos.inverse());
}

void entityTank::spawnTransition()
{
	entity::spawnTransition();
	mAIState = game::mPlayers.mPlayer1.mPoints > 10000 ? AISTATE_APPROACHING : AISTATE_THINKING;
	mReloadTimer = mReloadTime;

	Point3d playerPos = game::mPlayers.mPlayer1.getPos();
	float distance = 140;
	float angle = mathutils::frandFrom0To1() * (2*PI);
	Point3d spawnVector(0,0,distance);
	spawnVector = mathutils::rotate2dPoint(spawnVector, angle);

	mHeading = mathutils::frandFrom0To1() * (2*PI);

	mPos = playerPos + spawnVector;
	mPos.y = 0;

	mInitialFiringTimer = 100;
}

void entityTank::runBehavior()
{
	//
	// Get the angle from this tank to us
	//

	Point3d v1 = game::mPlayers.mPlayer1.getPos();
	Point3d v2 = mPos;

	float angle = fmod((double)mathutils::calculate2dAngle(v1, v2), 2*PI);
	float heading = fmod((double)mHeading, 2*PI);

	float targetingAngle = mathutils::diffAngles(angle, heading);
	float targetingOffset = fabs(targetingAngle);

	//
	// Get the distance from this tank to us
	//
	float targetingDistance = mathutils::calculate2dDistance(v1, v2);
	if (targetingDistance > 600)
	{
		// Too far away from the tank - reset with another enemy
		setState(entity::ENTITY_STATE_INACTIVE);
		return;
	}

	BOOL turning = FALSE;
	float rotationRate = 0;
	float speed = 0;


	if (mAIState == AISTATE_THINKING)
	{
		mAIState = (targetingAngle > 0) ? AISTATE_AIMING_LEFT : AISTATE_AIMING_RIGHT;
	}
	else if (mAIState == AISTATE_AIMING_LEFT)
	{
		rotationRate = 1;
		speed = 0;

		if (targetingOffset < 0.04)
		{
			mAIState = AISTATE_APPROACHING;
		}
	}
	else if (mAIState == AISTATE_AIMING_RIGHT)
	{
		rotationRate = -1;
		speed = 0;

		if (targetingOffset < 0.04)
		{
			mAIState = AISTATE_APPROACHING;
		}
	}
	else if (mAIState == AISTATE_APPROACHING)
	{
		speed = 1;

		rotationRate = targetingAngle * 20;

		if (targetingDistance < 20)
		{
			// Stop - close enough
			speed = 0;
		}

		if (targetingDistance > 400)
		{
			// Far away - turn in place to aim
			mAIState = AISTATE_THINKING;
		}
	}
	else if (mAIState == AISTATE_OBSTRUCTIONAVOID_LEFT)
	{
		rotationRate = 1;
		speed = -1;
		if (--mAIStateTimer <= 0)
		{
			mAIStateTimer = 200;
			mAIState = AISTATE_OBSTRUCTIONAVOID_FORWARD;
		}
	}
	else if (mAIState == AISTATE_OBSTRUCTIONAVOID_RIGHT)
	{
		rotationRate = -1;
		speed = -1;
		if (--mAIStateTimer <= 0)
		{
			mAIStateTimer = 200;
			mAIState = AISTATE_OBSTRUCTIONAVOID_FORWARD;
		}
	}
	else if (mAIState == AISTATE_OBSTRUCTIONAVOID_FORWARD)
	{
		rotationRate = 0;
		speed = 1;
		if (--mAIStateTimer <= 0)
		{
			mAIState = AISTATE_THINKING;
		}
	}
	else
	{
//		OutputDebugString(L"UNKNOWN STATE\n");
	}


	mHeading += rotationRate * mTurnFactor;
	mHeading = fmod((double)mHeading, 2*PI);

	Point3d vector(-speed * mSpeedFactor, 0, 0);
	vector = mathutils::rotate2dPoint(vector, mHeading);
	mPos.x += vector.x;
	mPos.z += vector.z;


	// Check for nearby obstructions
	entity* e = game::mObstructions.hitTestObstructionsAtPoint(mPos, mRadius);
	entityObstruction* ob = dynamic_cast<entityObstruction*>(e);
	if (ob && ob->mObstructsMovement)
	{
		Point3d v1 = e->getPos();
		Point3d v2 = mPos;

		float angle = fmod((double)mathutils::calculate2dAngle(v1, v2), 2*PI);
		float heading = fmod((double)mHeading, 2*PI);

		float obstructionAngle = mathutils::diffAngles(angle, heading);
		float obstructionOffset = fabs(obstructionAngle);

		if (obstructionAngle > 0)
			mAIState = AISTATE_OBSTRUCTIONAVOID_RIGHT;
		else
			mAIState = AISTATE_OBSTRUCTIONAVOID_LEFT;

		mAIStateTimer = 200;
	}

	// Take a shot if we have it
	--mInitialFiringTimer;
	if (mInitialFiringTimer < 0) mInitialFiringTimer = 0;
	if (mInitialFiringTimer <= 0)
	{
		--mReloadTimer;
		if (mReloadTimer < 0) mReloadTimer = 0;
		if (game::mPlayers.mPlayer1.getState() == entity::ENTITY_STATE_RUNNING)
		{
			if (targetingOffset < 0.1)
			{
				if (mReloadTimer <= 0)
				{
					game::mSound.playTrack(SOUNDID_ENEMY_FIRE);

					mReloadTimer = mReloadTime;

					// Fire a projectile
					entityProjectile* projectile = game::mProjectiles.getUnusedProjectile();
					if (projectile)
					{
						projectile->setState(entity::ENTITY_STATE_RUNNING);
						projectile->mSource = entityProjectile::PROJECTILE_SOURCE_ENEMY;
						projectile->mDurationTimer = 0;
						projectile->setHeading(mHeading);

						Point3d spawnPoint(mPos);
						spawnPoint.y = -6;
						projectile->setPos(spawnPoint);

						const float projectileSpeed = 3;
						Point3d vector(0, 0, -projectileSpeed);
						vector = mathutils::rotate2dPoint(vector, mHeading-mathutils::DegreesToRads(90));
						projectile->setSpeed(vector);

						// Recoil
						mTargetRotation.z = 1;
					}

				}
			}
		}
	}

/*

	// Rock back and forth
	{    
//		float angle = mSpeedDiff * .7;
//		mTargetRotation.x = angle;

		mRotationRate.z += (mTargetRotation.z - mRotation.z) * .008;
		mRotation.z += mRotationRate.z;
	}

	mRotationRate.z *= .98;

	mRotationForeAndAft = mRotation.z;

	mTargetRotation.z *= .9;
*/
}

