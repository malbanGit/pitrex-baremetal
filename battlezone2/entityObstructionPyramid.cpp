#include "entityObstructionPyramid.h"
#include "objloader.h"

entityObstructionPyramid::entityObstructionPyramid()
	: entityObstruction()
{
	mType = ENTITY_TYPE_OBSTRUCTION_PYRAMID;

	mModel = objloader::loadFile((char*)"models/obstruction_pyramid.obj");
	model::flipXY(mModel);

	mScale = 8;
	mRadius = 6;
	mHeading = 0;

	mPos = Point3d(0,0,0);

	mPen = vector::penGreen();
	mPen.a *= 1.1;

	mObstructsFire = TRUE;
	mObstructsMovement = TRUE;
}

entityObstructionPyramid::~entityObstructionPyramid()
{
	delete mModel;
}
