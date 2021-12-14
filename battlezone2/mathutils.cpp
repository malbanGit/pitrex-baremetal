#include "mathutils.h"

namespace mathutils
{

    float calculate2dDistance(Point3d p1, Point3d p2)
    {
        float x1 = p1.x;
        float z1 = p1.z;
        float x2 = p2.x;
        float z2 = p2.z;

        float dx = x1 - x2;
        float dz = z1 - z2;
        float distance = sqrt((dx*dx) + (dz*dz));

        return distance;
    }

    float calculateScreenDistance(Point3d p1, Point3d p2)
    {
        float x1 = p1.x;
        float y1 = p1.y;
        float x2 = p2.x;
        float y2 = p2.y;

        float dx = x1 - x2;
        float dy = y1 - y2;
        float distance = sqrt((dx*dx) + (dy*dy));

        return distance;
    }

	float calculateRayPointAngle(Point3d* v1, Point3d* v2, Point3d* point)
	{
	  float rayAngle = calculate2dAngle(*v1, *v2);
	  float rayToPointAngle = calculate2dAngle(*v1, *point);

	  return diffAngles(rayAngle, rayToPointAngle);
	}

    float calculate2dAngle(Point3d from, Point3d to)
    {
        float angle = 0.0;

        float x1 = from.x;
        float z1 = from.z;
        float x2 = to.x;
        float z2 = to.z;

        float dx = x2 - x1;
        float dz = z2 - z1;

        if (dx == 0)
        {
            if (dz == 0)
                angle = 0;
            else if (dz > 0)
                angle = PI/2;
            else
                angle = PI * 3.0 / 2.0;
        }
        else if (dz == 0)
        {
            if (dx > 0)
                angle = 0;
            else
                angle = PI;
        }
        else
        {
            if (dx < 0)
                angle = atan(dz/dx) + PI;
            else if (dz < 0)
                angle = atan(dz/dx) + (2*PI);
            else
                angle = atan(dz/dx);
        }

        if (angle >= RADIAN) angle-=RADIAN;

        return angle;
    }

    float calculateScreenAngle(Point3d from, Point3d to)
    {
        float angle = 0.0;

        float x1 = from.x;
        float y1 = from.y;
        float x2 = to.x;
        float y2 = to.y;

        float dx = x2 - x1;
        float dy = y2 - y1;

        if (dx == 0)
        {
            if (dy == 0)
                angle = 0;
            else if (dy > 0)
                angle = PI/2;
            else
                angle = PI * 3.0 / 2.0;
        }
        else if (dy == 0)
        {
            if (dx > 0)
                angle = 0;
            else
                angle = PI;
        }
        else
        {
            if (dx < 0)
                angle = atan(dy/dx) + PI;
            else if (dy < 0)
                angle = atan(dy/dx) + (2*PI);
            else
                angle = atan(dy/dx);
        }

        if (angle >= RADIAN) angle-=RADIAN;

        return angle;
    }

    float diffAngles(float angle1, float angle2)
    {
      angle1 = fmod(angle1, mathutils::DegreesToRads(360));
      angle2 = fmod(angle2, mathutils::DegreesToRads(360));

      float angle = angle1-angle2;

      if (angle >= mathutils::DegreesToRads(180))
        angle -= mathutils::DegreesToRads(360);
      else if (angle <= -mathutils::DegreesToRads(180))
        angle += mathutils::DegreesToRads(360);

      return angle;
    }

    Point3d clamp2dVector(const Point3d& vector, float max)
    {
        Point3d clamped = vector;
        float distance = mathutils::calculate2dDistance(Point3d(0,0,0), clamped);
        if (distance > max)
        {
            float r = max / distance;
            clamped.x *= r;
            clamped.y *= r;
        }
        return clamped;
    }

    Point3d rotate2dPoint(const Point3d& point, float angle)
    {
        float normalx = point.x;
        float normalz = point.z;
        Point3d pt(normalx * cos(angle) - normalz * sin(angle), 0, normalx * sin(angle) + normalz * cos(angle));
        return pt;
    }

    Point3d translate2dPoint(const Point3d& point, float tx, float tz)
    {
        Point3d pt(point.x + tx, point.y, point.z + tz);
        return pt;
    }

    Point3d translate2dPoint(const Point3d& point, Point3d translation)
    {
        Point3d pt(point.x + translation.x, point.y, point.z + translation.z);
        return pt;
    }

    Point3d rotateScreenPoint(const Point3d& point, float angle)
    {
        float normalx = point.x;
        float normaly = point.y;
        Point3d pt(normalx * cos(angle) - normaly * sin(angle), normalx * sin(angle) + normaly * cos(angle), 0);
        return pt;
    }

    Point3d translateScreenPoint(const Point3d& point, float tx, float ty)
    {
        Point3d pt(point.x + tx, point.y + ty, point.z);
        return pt;
    }

    Point3d translateScreenPoint(const Point3d& point, Point3d translation)
    {
        Point3d pt(point.x + translation.x, point.y + translation.y, point.z);
        return pt;
    }

    Point3d translate2dPointFromPointByRadius(const Point3d& point, const Point3d& fromPoint, float distance)
    {
        float angle = mathutils::calculate2dAngle(point, fromPoint);
//        float distance = mathutils::calculate2dDistance(point, fromPoint);

        Point3d vPoint(distance,0,0);
        Point3d rPoint = mathutils::rotate2dPoint(vPoint, angle);

        return rPoint;
    }

    int randFromTo(int from, int to)
    {
        int range = to-from;
        return (int) ((frandFrom0To1()*(range+1))+from);
    }

    float frandFrom0To1()
    {
        return (float)rand()/RAND_MAX;
    }

};


