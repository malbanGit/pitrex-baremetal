#include "entityMissile.h"
#include "objloader.h"
#include "game.h"
#include "entityObstruction.h"


entityMissile::entityMissile()
{
	mType = ENTITY_TYPE_MISSILE;

	mPointValue = 2000;

	mModel = objloader::loadFile((char*)"models/missile.obj");
	model::flipXY(mModel);

	mScale = 2.5;
	mRadius = 5;
	mModel->Scale(2.5);

	mSpawnTime = 70;
	mDestroyTime = 150;
	mReloadTime = 100;

	mPen = vector::penGreen();
	mPen.a *= 1.2;

    mPatternIndex = 0;

	mCurrentAltitude = mTargetAltitude = -200;
	setAltitude(mCurrentAltitude);

	mAIState = AISTATE_THINKING;

	setState(entity::ENTITY_STATE_INACTIVE);
}

entityMissile::~entityMissile()
{
}

void entityMissile::draw(const camera& cam)
{
	if (getState() == entity::ENTITY_STATE_RUNNING)
	{
		entity::draw(cam);
	}
}

void entityMissile::spawnTransition()
{
	entity::spawnTransition();

	float distance = 500;

	Point3d playerPos = game::mPlayers.mPlayer1.getPos();
	mAngleFromPlayer = game::mPlayers.mPlayer1.getHeading() + mathutils::DegreesToRads(180);
	Point3d spawnVector(0,0,distance);
	spawnVector = mathutils::rotate2dPoint(spawnVector, mAngleFromPlayer);

	mPos = playerPos + spawnVector;

	mHeading = mathutils::calculate2dAngle(playerPos, mPos);

	mCurrentAltitude = mTargetAltitude = -200;
	setAltitude(mCurrentAltitude);

	mCurrentTurnDirection = mTargetTurnDirection = 0;
	mTurnTimer = 0;
    mTurnIndex = 0;

	mOvershoot = FALSE;

	game::mSound.playTrack(SOUNDID_MISSILE_LOOP);
}

void entityMissile::run()
{
	runBehavior();
}

void entityMissile::destroyTransition()
{
	entity::destroyTransition();
	game::mSound.stopTrack(SOUNDID_MISSILE_LOOP);
    mPatternIndex = (mathutils::frandFrom0To1() * 2) + 1;
}

void entityMissile::runBehavior()
{
/*
	// Throw out some particles
	Point3d pos(mPos.inverse());
	pos.y = 5;
	Point3d angle(0,0,0);
	float speed = .2;
	float spread = 2*PI;
	int num = 100;
	int timeToLive = 300;
	vector::pen pen = getPen();
	pen.a = 10;
	pen.lineRadius=4;
	game::mParticles.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, FALSE, .90, FALSE);
*/

    int pattern0[] = 
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        2
    };

    int pattern1[] = 
    {
        0,
        1,
        -1,
        0,
        0,
        -1,
        1,
        0,
        2
    };

    int pattern2[] = 
    {
        0,
        -1,
        -1,
        1,
        1,
        1,
        2
    };

    int *pattern;
    switch (mPatternIndex)
    {
        default:
        case 0:
            pattern = pattern0;
            break;
        case 1:
            pattern = pattern1;
            break;
        case 2:
            pattern = pattern2;
            break;
    }

	// Check for nearby obstructions
	entity* e = game::mObstructions.hitTestObstructionsAtPoint(mPos, mRadius*3);
	entityObstruction* ob = dynamic_cast<entityObstruction*>(e);
	if (ob && ob->mObstructsMovement)
	{
		Point3d v1 = e->getPos();
		Point3d v2 = mPos;

		mTargetAltitude = -20;
	}
	else
	{
		mTargetAltitude = 0;
	}

	mCurrentAltitude += (mTargetAltitude - mCurrentAltitude) * .1;

	setAltitude(mCurrentAltitude);


	if ((mTargetAltitude == 0) && (mCurrentAltitude > -.1))
	{
		mCurrentAltitude = 0;
	}

	if (mOvershoot)
	{
		// Keep on flying away
		float distance = mathutils::calculate2dDistance(game::mPlayers.mPlayer1.getPos(), mPos);
		if (distance > 300)
		{
			// Despawn it
			setState(entity::ENTITY_STATE_INACTIVE);
			game::mSound.stopTrack(SOUNDID_MISSILE_LOOP);
		}
	}
	else
	{
		// Point at the player
		mHeading = mathutils::calculate2dAngle(game::mPlayers.mPlayer1.getPos(), mPos) + mCurrentTurnDirection;
		mHeading = fmod((double)mHeading, 2*PI);

		++mTurnTimer;
        if (mTurnTimer > 35)
        {
            mTurnTimer = 0;
            ++mTurnIndex;
            if (pattern[mTurnIndex] == 2)
            {
                --mTurnIndex;
            }
        }

		float distance = mathutils::calculate2dDistance(game::mPlayers.mPlayer1.getPos(), mPos);

		if (distance < 120)
		{
			mTargetTurnDirection = 0;

			if (distance < 10)
			{
				if (mCurrentAltitude < -1)
				{
					mOvershoot = TRUE;
				}
				else
				{
					game::mSound.stopTrack(SOUNDID_MISSILE_LOOP);
					game::mSound.playTrack(SOUNDID_PLAYER_EXPLODE);
					game::mSound.playTrack(SOUNDID_ENEMY_EXPLODE);

					// Hit the player
					game::mPlayers.mPlayer1.setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

					// Explode wreckage
					game::mEnemies.explodeEnemy(this);

					// Destroy ourselves
					setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

					// Throw out some particles
					Point3d pos(mPos.inverse());
					Point3d angle(0,0,0);
					float speed = .6;
					float spread = 2*PI;
					int num = 100;
					int timeToLive = 300;
					vector::pen pen = getPen();
					pen.a = 1;
					pen.lineRadius=4;
					game::mParticles.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, FALSE, .99, FALSE);
				}
			}
		}
		else
		{
			switch(pattern[mTurnIndex])
			{
				case 1:
					mTargetTurnDirection = mathutils::DegreesToRads(90);
					break;
				case -1:
					mTargetTurnDirection = mathutils::DegreesToRads(-90);
					break;
				default:
					mTargetTurnDirection = mathutils::DegreesToRads(0);
					break;
			}
		}

		mCurrentTurnDirection += (mTargetTurnDirection - mCurrentTurnDirection) * .2;
	}

	// Move forwards
	Point3d vector(2.2, 0, 0);
	vector = mathutils::rotate2dPoint(vector, mHeading + mathutils::DegreesToRads(180));
	mPos.x += vector.x;
	mPos.z += vector.z;

}

void entityMissile::setAltitude(float alt)
{
	mPos.y = (getModel()->getBottom() * getScale().y) + alt;
}


entity* entityMissile::hitTest(const Point3d& pos, float radius)
{
	if (mCurrentAltitude > -5)
	{
		return entity::hitTest(pos, radius);
	}
    return NULL;
}






