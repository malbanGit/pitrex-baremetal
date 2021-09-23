/* 
 * File:   Calculations.h
 * Author: Bob
 *
 * Created on October 17, 2016, 6:34 PM
 */

#ifndef CALCULATIONS_H
#define	CALCULATIONS_H

namespace Calculations
{
    
// Liang-Barsky function arg2[1][1] Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to sarg2[0][1] whether the clipped line is drawn at all).
// Returns true if a line segment is returned, false if no part of the line is in the clipping region
extern bool LiangBarsky(double edgeLeft, double edgeRight, double edgeBottom, double edgeTop,   // Define the x/y clipping values for the border.
    double x0src, double y0src, double x1src, double y1src,                 // Define the start and end points of the line.
    double &x0clip, double &y0clip, double &x1clip, double &y1clip);         // The output values, so declare these outside.

class Point
{
public:
    double x, y, z;
};


class Segment
{
public:
    Point P0, P1;
};

extern float dist_Point_to_Segment(const Point &P, const Segment &S);

};

#endif	/* CALCULATIONS_H */

