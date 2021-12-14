#include "entityLogo.h"
#include "objloader.h"
#include "game.h"


entityLogo::entityLogo()
{
    mType = ENTITY_TYPE_LOGO;

    mModel = objloader::loadFile((char*)"models/logo.obj");
    mScale = 40;
    mModel->Scale(40);

    mPen = vector::penGreen();
    mPen.a *= 1.5;

    setState(entity::ENTITY_STATE_INACTIVE);

    mPos = Point3d(0, 50, 0);
    setState(entity::ENTITY_STATE_INACTIVE);
    mHeading = mathutils::DegreesToRads(-90);
}

entityLogo::~entityLogo()
{
}

void entityLogo::run()
{
    if (theScene->mShowingLogo)
    {
        setState(entity::ENTITY_STATE_RUNNING);
        mPos.z -= .6;
        mPos.y -= .05;
    }
    else
    {
        setState(entity::ENTITY_STATE_INACTIVE);
        mPos = Point3d(0, 50, 0);
    }

    entity::run();
}

void entityLogo::draw(const camera& cam)
{
    entity::draw(cam);
}


