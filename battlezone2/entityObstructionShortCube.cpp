#include "entityObstructionShortCube.h"
#include "objloader.h"


entityObstructionShortCube::entityObstructionShortCube()
	: entityObstruction()
{
	mType = ENTITY_TYPE_OBSTRUCTION_SHORTCUBE;

	mModel = objloader::loadFile((char*)"models/obstruction_shortcube.obj");
	model::flipXY(mModel);

	mScale = 8;
	mRadius = 6;
	mHeading = 0;

	mPos = Point3d(0,0,0);

	mPen = vector::penGreen();
	mPen.a *= 1.1;

	mObstructsFire = FALSE;
	mObstructsMovement = TRUE;

}

entityObstructionShortCube::~entityObstructionShortCube()
{
	delete mModel;
}
