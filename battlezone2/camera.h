#ifndef CAMERA_H
#define CAMERA_H

#include "point3d.h"
#include "matrix.h"

class camera
{
  public:
    camera();

    void run();

    matrix mMatrix;

    Point3d mPos;
    float mHeading;
    float mRotationForeAndAft;
    float mRotationSideToSide;
};

#endif // CAMERA_H
