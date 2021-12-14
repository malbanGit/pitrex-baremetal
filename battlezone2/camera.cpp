#include "camera.h"
#include "game.h"


camera::camera()
{
}

void camera::run()
{
	mMatrix.Identity();
	mMatrix.Translate(mPos.x, mPos.y, mPos.z);
	mMatrix.Rotate(0,mHeading,0);
	mMatrix.Rotate(0,0,mRotationSideToSide);
	mMatrix.Rotate(mRotationForeAndAft,0,0);
}

