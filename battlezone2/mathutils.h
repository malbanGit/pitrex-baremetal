#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "defines.h"
#include "point3d.h"

namespace mathutils
{
    float calculate2dDistance(Point3d p1, Point3d p2);

	float calculateRayPointAngle(Point3d* v1, Point3d* v2, Point3d* point);

    float calculate2dAngle(Point3d from, Point3d to);

    float calculateScreenDistance(Point3d p1, Point3d p2);

    float calculateScreenAngle(Point3d from, Point3d to);

    float diffAngles(float angle1, float angle2);

    Point3d clamp2dVector(const Point3d& vector, float max);

    Point3d rotate2dPoint(const Point3d& point, float angle);

    Point3d translate2dPoint(const Point3d& point, float tx, float tz);

    Point3d translate2dPoint(const Point3d& point, Point3d translation);

    Point3d rotateScreenPoint(const Point3d& point, float angle);

    Point3d translateScreenPoint(const Point3d& point, float tx, float ty);

    Point3d translateScreenPoint(const Point3d& point, Point3d translation);

    Point3d translate2dPointFromPointByRadius(const Point3d& point, const Point3d& fromPoint, float distance);

    int randFromTo(int from, int to);

    float frandFrom0To1();

    inline float RadsToDegrees(float radians)
    {
      return radians * 57.2957795786;
    }

    inline float DegreesToRads(float degrees)
    {
      return degrees * 0.0174532925;
    }

};

#endif // MATHUTILS_H
