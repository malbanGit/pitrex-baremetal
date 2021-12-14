#include "player.h"
#include "game.h"
#include "entityObstruction.h"

#define FIELD_SIZE 500

#define SLOW_SPEED .5
#define FAST_SPEED 1.0
#define STRAIN_SPEED 1.2

player::player()
{
    mHeading = 0;
    mRotationForeAndAft = 0;
    mRotationSideToSide = 0;
    mProjectile = NULL;
    mPoints = 0;
    mLives = 0;

    mSpawnTime = 0;
    mDestroyTime = 150;
}

player::~player()
{
}

void player::startGame()
{
	mLives = 3;
	mPoints = 0;
	mState = entity::ENTITY_STATE_SPAWN_TRANSITION;

	if (!game::mSound.isTrackPlaying(SOUNDID_PLAYER_ENGINE_LOOP))
		game::mSound.playTrack(SOUNDID_PLAYER_ENGINE_LOOP);

	mCurrentEngineSpeed = mTargetEngineSpeed = SLOW_SPEED;
}

void player::spawnTransition()
{
	entity::spawnTransition();

	float x = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
	float y = (mathutils::frandFrom0To1() * FIELD_SIZE)-(FIELD_SIZE/2);
	setPos(Point3d(x, 0, y));

	game::mEnemies.disableAllEnemies();
}

void player::run()
{
	// Track previous projectiles
	if (mProjectile)
	{
		if ((mProjectile->getState() == entity::ENTITY_STATE_INACTIVE)
			|| (mProjectile->mSource != entityProjectile::PROJECTILE_SOURCE_PLAYER1))
		{
			// It's used up - enable firing again
			// This is how we only allow the player to only have a single active projectile at a time
			mProjectile = NULL;
		}
	}

	if (game::mGameMode == game::GAMEMODE_PLAYING)
	{
		mReloading = mProjectile ? TRUE : FALSE;

		if (mState == entity::ENTITY_STATE_RUNNING)
		{
			BOOL trigger = game::mControls.getTriggerButton(0);
			if (trigger)
			{
				// Fire a projectile
				if (!mProjectile)
				{
					entityProjectile* projectile = game::mProjectiles.getUnusedProjectile();
					if (projectile)
					{
						game::mSound.playTrack(SOUNDID_PLAYER_FIRE);

						mProjectile = projectile;

						projectile->setState(entity::ENTITY_STATE_RUNNING);
						projectile->mSource = entityProjectile::PROJECTILE_SOURCE_PLAYER1;
						projectile->mDurationTimer = 0;
						projectile->setHeading(mHeading+mathutils::DegreesToRads(90));

						// Place the projectile out in front of the tank
						Point3d spawnPoint(0,0,-2);
						spawnPoint = mathutils::rotate2dPoint(spawnPoint, mHeading);

						spawnPoint += mPos;
						projectile->setPos(spawnPoint);

						const float projectileSpeed = 3;
						Point3d vector(0, 0, -projectileSpeed);
						vector = mathutils::rotate2dPoint(vector, mHeading);
						projectile->setSpeed(vector);

						// Recoil
//						mRotationRate.x += .01;
					}
				}
			}

			int leftDir=0, rightDir=0;

			Point3d leftStick = game::mControls.getLeftStick(0);
			float distance = mathutils::calculate2dDistance(Point3d(0,0,0), leftStick);
			if (distance > .1)
			{
				float angle = mathutils::RadsToDegrees(mathutils::calculate2dAngle(Point3d(0,0,0), leftStick));

				if ((angle < 30) || (angle > 330))
					leftDir = -1;
				else if ((angle > 150) && (angle < 210))
					leftDir = 1;
			}

			Point3d rightStick = game::mControls.getRightStick(0);
			distance = mathutils::calculate2dDistance(Point3d(0,0,0), rightStick);
			if (distance > .1)
			{
				float angle = mathutils::RadsToDegrees(mathutils::calculate2dAngle(Point3d(0,0,0), rightStick));

				if ((angle < 30) || (angle > 330))
					rightDir = -1;
				else if ((angle > 150) && (angle < 210))
					rightDir = 1;
			}


			runDigitalSteering(leftDir, rightDir);


			// Move forwards in the direction of the tank
			Point3d vector(0, 0, mForwardSpeed*.8);
			vector = mathutils::rotate2dPoint(vector, mHeading);
			Point3d tempPos = mPos + vector;

			BOOL hitSomething = FALSE;

			//
			// Initial hit tests
			//

			// Hit test against obstructions
			entity* e = game::mObstructions.hitTestObstructionsAtPoint(tempPos, mRadius);
			entityObstruction* ob = dynamic_cast<entityObstruction*>(e);
			if (ob && ob->mObstructsMovement)
			{
				mForwardSpeed = 0;
				mSpeedDiff *= -.2;
				hitSomething = TRUE;
			}

			// Hit test against enemy tanks
			entity* en = game::mEnemies.hitTestEnemiesAtPoint(tempPos, 10);
			if (en && (en->getType() != entity::ENTITY_TYPE_SAUCER) && (en->getType() != entity::ENTITY_TYPE_MISSILE))
			{
				mForwardSpeed = 0;
				mSpeedDiff *= -.2;
				hitSomething = TRUE;
			}

			if (!hitSomething)
			{
				mPos = tempPos;
			}

			//
			// Hit test a little bigger to see if we're still blocked
			//

			BOOL motionBlocked = FALSE;

			// Obstructions
			e = game::mObstructions.hitTestObstructionsAtPoint(mPos, mRadius+.8);
			ob = dynamic_cast<entityObstruction*>(e);
			if (ob && ob->mObstructsMovement)
			{
				motionBlocked = TRUE;
			}

			// Enemies
			en = game::mEnemies.hitTestEnemiesAtPoint(tempPos, 10+.8);
			if (en && (en->getType() != entity::ENTITY_TYPE_SAUCER) && (en->getType() != entity::ENTITY_TYPE_MISSILE))
			{
				motionBlocked = TRUE;
			}

			if (hitSomething && !mHitSomething)
			{
				if (!game::mSound.isTrackPlaying(SOUNDID_HIT_OBSTRUCTION))
					game::mSound.playTrack(SOUNDID_HIT_OBSTRUCTION);
			}

			mHitSomething = hitSomething;
			mMotionBlocked = motionBlocked;
		}
		else
		{
			mHeadingDiff = 0;
			mSpeedDiff = 0;
			mForwardSpeed = 0;
		}

		if (mHitSomething)
			mTargetEngineSpeed = STRAIN_SPEED;
		else
			mTargetEngineSpeed = (mForwardSpeed || mHeadingDiff) ? FAST_SPEED : SLOW_SPEED;

		if (mCurrentEngineSpeed < mTargetEngineSpeed)
		{
			mCurrentEngineSpeed += .02;
			if (mCurrentEngineSpeed > mTargetEngineSpeed)
				mCurrentEngineSpeed = mTargetEngineSpeed;
		}
		else if (mCurrentEngineSpeed > mTargetEngineSpeed)
		{
			mCurrentEngineSpeed -= .02;
			if (mCurrentEngineSpeed < mTargetEngineSpeed)
				mCurrentEngineSpeed = mTargetEngineSpeed;
		}

		game::mSound.setTrackSpeed(SOUNDID_PLAYER_ENGINE_LOOP, mCurrentEngineSpeed);

		if (mMotionBlocked)
		{
			if (!game::mSound.isTrackPlaying(SOUNDID_OBSTRUCTION_WARNING))
				game::mSound.playTrack(SOUNDID_OBSTRUCTION_WARNING);
		}
		else
		{
			if (game::mSound.isTrackPlaying(SOUNDID_OBSTRUCTION_WARNING))
				game::mSound.stopTrack(SOUNDID_OBSTRUCTION_WARNING);
		}

		// Put the camera up a bit
		mPos.y = -5;

		// Rock back and forth
		{    
			float angle = mSpeedDiff * .4;
			mTargetRotation.x = angle;

			mRotationRate.x += (mTargetRotation.x - mRotation.x) * .008;
			mRotation.x += mRotationRate.x;
		}

		// Rock side to side
		{    
			float angle = -mHeadingDiff * 6 * mForwardSpeed;
			mTargetRotation.z = angle;

			mRotationRate.z += (mTargetRotation.z - mRotation.z) * .008;
			mRotation.z += mRotationRate.z;
		}

		mRotationRate.x *= .94;
		mRotationRate.y *= .94;
		mRotationRate.z *= .94;

		mRotationForeAndAft = mRotation.x;
		mRotationSideToSide = mRotation.z;

	}
	else
	{
		if (theScene->mShowingLogo)
		{
		    mPos = Point3d(0, 0, 0);
		    mHeading = 0;
		    mHeadingDiff = 0;
		}
		else
		{
		    if (--mAttractModeTurnTimer <= 0)
		    {
			mAttractModeTurnTimer = 200;
			mAttractModeTurnType = mathutils::frandFrom0To1() * 6;
		    }

		    switch (mAttractModeTurnType)
		    {
		    case 0:
			runDigitalSteering(0, -1);
			break;
		    case 1:
			runDigitalSteering(-1, 0);
			break;
		    case 2:
			runDigitalSteering(-1, -1);
			break;
		    case 3:
			runDigitalSteering(1, 0);
			break;
		    case 4:
			runDigitalSteering(0, 1);
			break;
		    case 5:
			runDigitalSteering(1, 1);
			break;
		    }

		    // Move forwards in the direction of the tank
		    Point3d vector(0, 0, mForwardSpeed*.8);
		    vector = mathutils::rotate2dPoint(vector, mHeading);
		    mPos += vector;
		}

		// Put the camera up a bit
		mPos.y = -5;

		mTargetRotation.x = 0;
		mTargetRotation.y = 0;
		mTargetRotation.z = 0;

		mRotation.x = 0;
		mRotation.y = 0;
		mRotation.z = 0;

		mRotationRate.x = 0;
		mRotationRate.y = 0;
		mRotationRate.z = 0;

		mRotationForeAndAft = 0;
		mRotationSideToSide = 0;
	}

	//
	// Align the camera with the player
	//
	mCamera.mPos = mPos;
	mCamera.mHeading = mHeading;
	mCamera.mRotationForeAndAft = mRotationForeAndAft;
	mCamera.mRotationSideToSide = mRotationSideToSide;
	mCamera.run();
}

void player::destroyTransition()
{
	entity::destroyTransition();

	mHeadingDiff = 0;
	mSpeedDiff = 0;
	mForwardSpeed = 0;

	mCrackDistance = 0;

	--mLives;
}

void player::destroy()
{
    if (--mStateTimer <= 0)
    {
        setState(ENTITY_STATE_INACTIVE);
		mCrackDistance = 0;

		if (mLives <= 0)
		{
			// We're dead - shut down and signal game over

			mAttractModeTurnTimer = 0;
			mAttractModeTurnType = 0;

			mMotionBlocked = FALSE;
			mHitSomething = FALSE;

			game::mSound.stopTrack(SOUNDID_PLAYER_ENGINE_LOOP);

			game::mEnemies.disableAllEnemies();

			game::mGameMode = game::GAMEMODE_HIGHSCORES_CHECK;
		}

		return;
    }
    run();
}

void player::draw()
{
	if (game::mGameMode == game::GAMEMODE_PLAYING)
	{
		if (mState == entity::ENTITY_STATE_DESTROYED)
			drawWindowCracks();
	}
}

void player::drawWindowCracks()
{
	const int numLines = 37;

	float crackData[numLines][4] =
	{
		{ 3.375, 795.57, 85.375, 719.57 },
		{ 85.375, 719.57, 85.375, 701.57 },
		{ 85.375, 719.57, 113.875, 701.57 },
		{ 3.375, 795.57, 57.875, 825.07 },
		{ 57.875, 825.07, 100.875, 825.07 },
		{ 100.875, 825.07, 100.875, 849.57 },
		{ 100.875, 849.57, 160.375, 849.57 },
		{ 160.375, 849.57, 217.375, 789.57 },
		{ 217.375, 789.57, 269.375, 856.07 },
		{ 269.375, 856.07, 281.375, 879.07 },
		{ 281.375, 879.07, 298.375, 916.57 },
		{ 269.375, 856.07, 332.375, 810.32 },
		{ 281.375, 879.07, 325.875, 861.07 },
		{ 160.375, 849.57, 172.875, 885.07 },
		{ 160.375, 849.57, 154.875, 885.07 },
		{ 154.875, 885.07, 125.875, 916.57 },
		{ 3.375, 795.57, -19.625, 856.07 },
		{ -19.625, 856.07, 17.375, 885.07 },
		{ 17.375, 885.07, 41.375, 969.07 },
		{ 41.375, 969.07, -1.125, 1058.57 },
		{ -1.125, 1058.57, -14.125, 1073.57 },
		{ -1.125, 1058.57, 3.375, 1073.57 },
		{ 3.375, 795.57, -52.625, 734.57 },
		{ -52.625, 734.57, -133.625, 707.07 },
		{ -133.625, 707.07, -206.125, 779.57 },
		{ -52.625, 734.57, -19.625, 701.57 },
		{ 3.375, 795.57, -39.625, 795.57 },
		{ -39.625, 795.57, -39.625, 822.82 },
		{ -39.625, 822.82, -88.125, 825.07 },
		{ -88.125, 825.07, -133.625, 810.32 },
		{ -88.125, 825.07, -88.125, 856.07 },
		{ -88.125, 856.07, -106.125, 873.57 },
		{ -106.125, 873.57, -133.625, 895.57 },
		{ -133.625, 895.57, -169.875, 906.07 },
		{ -169.875, 906.07, -222.625, 876.07 },
		{ -169.875, 906.07, -188.125, 935.57 },
		{ -106.125, 873.57, -79.125, 916.57 }
	};


	vector::pen pen = vector::penGreen();
	pen.a = .7;


	Point3d start, end;

	float vscale = 250;
	float hscale = 270;

	mCrackDistance += .05;

	// LINES

//    glColor4f(pen.r, pen.g, pen.b, pen.a);


	for (int i=0; i<numLines; i++)
	{
		start.x = crackData[i][0];
		start.y = crackData[i][1];
		end.x = crackData[i][2];
		end.y = crackData[i][3];

		start.x /= hscale;
		start.y /= vscale;
		end.x /= hscale;
		end.y /= vscale;

		Point3d mid((start.x + end.x) / 2, (start.y + end.y) / 2, 0);
		float distance = mathutils::calculate2dDistance(mid, Point3d(0,0,0));
		if (distance < mCrackDistance)
		{
			start.y -= 3.4;
			end.y -= 3.4;

			start.x -= .2;
			end.x -= .2;

			start.y = -start.y;
			end.y = -end.y;

drawLine(start.x, start.y, end.x, end.y, (int) (pen.r+pen.g+pen.b/3.0*127.0));
		}
	}

}

void player::runDigitalSteering(int leftStick, int rightStick)
{
	float rotation = 0;
	float speed = 0;

	float oldHeading = mHeading;

	if (leftStick == 0 && rightStick == 0)
	{
		// stand still :-)
	}
	else
	{
		if (leftStick == 0 && rightStick > 0)
		{
			// right tread moving forward - left tread stopped
			rotation = leftStick - rightStick;
			speed = 1;
		}
		else if (leftStick > 0 && rightStick == 0)
		{
			// left tread moving forward - right tread stopped
			rotation = leftStick - rightStick;
			speed = 1;
		}

		else if (leftStick == 0 && rightStick < 0)
		{
			// right tread moving backward - left tread stopped
			rotation = leftStick - rightStick;
			speed = -1;
		}
		else if (leftStick < 0 && rightStick == 0)
		{
			// left tread moving backward - right tread stopped
			rotation = leftStick - rightStick;
			speed = -1;
		}

		else if (leftStick > 0 && rightStick > 0)
		{
			// both treads moving forward
			rotation = leftStick - rightStick;
			speed = (leftStick + rightStick)/2;  
		}
		else if (leftStick < 0 && rightStick < 0)
		{
			// both treads moving backward
			rotation = rightStick - leftStick;
			speed = (rightStick + rightStick)/2;  
		}
		else if (leftStick < 0 && rightStick > 0)
		{
			// hard left
			rotation = leftStick - rightStick;
			speed = 0;
		}
		else if (leftStick > 0 && rightStick < 0)
		{
			// hard right
			rotation = leftStick - rightStick;
			speed = 0;
		}
	}

	// Set the heading
	mHeading += rotation * .003;
	mHeadingDiff = mHeading - oldHeading;

	mHeading = fmod((double)mHeading,2*PI);

	// Set the forward speed
	mSpeedDiff =  speed - mForwardSpeed;
	mForwardSpeed = speed;
}

bool player::isTargetingEnemy()
{
	for (int i=0; i<NUM_ENEMIES; i++)
	{
		enemy* e = game::mEnemies.mEnemies[i];
		if (e->getState() == entity::ENTITY_STATE_RUNNING)
		{
			Point3d enemyPos = e->getPos();
			float playerHeading = game::mPlayers.mPlayer1.getHeading() + mathutils::DegreesToRads(90);
			float targetAngle = mathutils::calculate2dAngle(enemyPos, game::mPlayers.mPlayer1.getPos());
			float diff = mathutils::diffAngles(playerHeading, targetAngle);
			if (fabs((double)diff) < .06)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int player::getTargetDirection()
{

	for (int i=0; i<NUM_ENEMIES; i++)
	{
		enemy* e = game::mEnemies.mEnemies[i];
		if ((e->getState() == entity::ENTITY_STATE_RUNNING)
			&& ((e->getType() == entity::ENTITY_TYPE_TANK) || (e->getType() == entity::ENTITY_TYPE_SUPERTANK)))
		{
			Point3d enemyPos = e->getPos();
			float playerHeading = game::mPlayers.mPlayer1.getHeading() + mathutils::DegreesToRads(90);
			float targetAngle = mathutils::calculate2dAngle(enemyPos, game::mPlayers.mPlayer1.getPos());
			float diff = mathutils::diffAngles(playerHeading, targetAngle);
			if (fabs((double)diff) < .5)
			{
				return 0;
			}
			else if (diff < 0)
			{
				return 1;
			}
			else if (diff > 0)
			{
				return -1;
			}
		}
	}

	return 0;
}

void player::addPoints(int points)
{
	int pointsLast = mPoints;
	mPoints += points;

	// bonus tank at 15000 and 100000
	BOOL extraLife = FALSE;
	if ((pointsLast < 15000) && (mPoints >= 15000))
	{
		extraLife = TRUE;
	}
	else if ((pointsLast < 100000) && (mPoints >= 100000))
	{
		extraLife = TRUE;
	}

	if (extraLife)
	{
		game::mSound.playTrack(SOUNDID_EXTRA_LIFE);
		++mLives;
	}
}

