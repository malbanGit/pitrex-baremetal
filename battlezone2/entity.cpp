#include "entity.h"
#include "game.h"

#include "entityObstructionCube.h"
#include "entityObstructionShortCube.h"
#include "entityObstructionPyramid.h"
#include "entityProjectile.h"


entity::entity()
    : mType(ENTITY_TYPE_UNDEF)
{
    mModel = NULL;
    mScale = 10;
    setState(ENTITY_STATE_INACTIVE);
    mSpawnTime = 70;
    mDestroyTime = 0;
    mStateTimer = 0;
    mAggression = 1;

    mPos = Point3d(0, 0, 0);
    mSpeed = Point3d(0,0,0);
    mHeading = 0;
    mRotation = Point3d(0,0,0);
    mRotationRate = Point3d(0,0,0);
    mTargetRotation = Point3d(0,0,0);

    mPen = vector::pen(1,1,1,1,2);
}

entity::~entity()
{
	if (mModel) delete mModel;
}


// Static class factory
entity* entity::createEntity(EntityType _entity)
{

    switch (_entity)
    {
        case ENTITY_TYPE_OBSTRUCTION_CUBE:
            return new entityObstructionCube();
        case ENTITY_TYPE_OBSTRUCTION_SHORTCUBE:
            return new entityObstructionShortCube();
        case ENTITY_TYPE_OBSTRUCTION_PYRAMID:
            return new entityObstructionPyramid();
        case ENTITY_TYPE_PROJECTILE:
            return new entityProjectile();
	default:
	    return new entity();
    }

    return 0;
}

void entity::draw(const camera& cam)
{
    if (mModel && getEnabled())
    {
	// Update the model's matrix
	mModel->Identity();
	mModel->Scale(mScale);
	mModel->Rotate(-mRotation.x, -mHeading, -mRotation.z);
	mModel->Translate(mPos.inverse());

        vector::pen pen = mPen;
        mModel->draw(cam, pen);
    }
}

void entity::runTransition()
{
    setState(ENTITY_STATE_RUNNING);

    run();
}

void entity::run()
{
	if (getEnabled())
	{
		mAggression += .0002;

		mPos += mSpeed;
		
		mHeading += mRotation.y;

		mRotation.x += mRotationRate.x;
		mRotation.y += mRotationRate.y;
		mRotation.z += mRotationRate.z;

		mRotationRate.x *= .9;
		mRotationRate.y *= .9;
		mRotationRate.z *= .9;
	}

	// Update the model's matrix
	if (mModel)
	{
		mModel->Identity();
		mModel->Scale(mScale);
		mModel->Rotate(-mRotation.x, -mHeading, -mRotation.z);
		mModel->Translate(mPos.inverse());
	}
}

void entity::spawnTransition()
{
	setState(ENTITY_STATE_SPAWNING);
	mStateTimer = mSpawnTime;
	mPos = Point3d(0,0,0);
	mSpeed = Point3d(0,0,0);
	mHeading = 0;
	mRotation = Point3d(0,0,0);
	mRotationRate = Point3d(0,0,0);
	mTargetRotation = Point3d(0,0,0);
	mAggression = 1;

	if (mModel)
	{
		mModel->Identity();
		mModel->Scale(mScale);
		mModel->Rotate(-mRotation.x, -mHeading, -mRotation.z);
		mModel->Translate(mPos.inverse());
	}

//    spawn();
}

void entity::spawn()
{
    if (--mStateTimer <= 0)
    {
        setState(ENTITY_STATE_RUNNING);
    }
}

void entity::destroyTransition()
{
    setState(ENTITY_STATE_DESTROYED);
    mStateTimer = mDestroyTime;

    destroy();
}

void entity::destroy()
{
    if (--mStateTimer <= 0)
    {
        setState(ENTITY_STATE_INACTIVE);
    }
}

void entity::hit(entity* aEntity)
{
    if (aEntity)
    {
    }
    setState(ENTITY_STATE_DESTROY_TRANSITION);
}

entity* entity::hitTest(const Point3d& pos, float radius)
{
    Point3d ourPos(0,0,0);
    this->getModel()->mMatrix.TransformVertex(ourPos, &ourPos);

    float distance = mathutils::calculate2dDistance(pos, ourPos.inverse());
    float resultRadius = getRadius() + radius;
    if (distance < resultRadius)
    {
        return this;
    }
    return NULL;
}





