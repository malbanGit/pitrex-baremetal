#include "entityObstructionCube.h"
#include "objloader.h"

entityObstructionCube::entityObstructionCube()
	: entityObstruction()
{
	mType = ENTITY_TYPE_OBSTRUCTION_CUBE;

	mModel = objloader::loadFile((char*)"models/obstruction_cube.obj");
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

entityObstructionCube::~entityObstructionCube()
{
	delete mModel;
}

