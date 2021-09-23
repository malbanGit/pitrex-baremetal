#include <math.h>
#include "Calculations.h"

namespace Calculations
{
    
// Liang-Barsky function arg2[1][1] Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to sarg2[0][1] whether the clipped line is drawn at all).
// Returns true if a line segment is returned, false if no part of the line is in the clipping region
bool LiangBarsky(double edgeLeft, double edgeRight, double edgeBottom, double edgeTop,   // Define the x/y clipping values for the border.
                 double x0src, double y0src, double x1src, double y1src,                 // Define the start and end points of the line.
                 double &x0clip, double &y0clip, double &x1clip, double &y1clip)         // The output values, so declare these outside.
{
    double t0 = 0.0;    
    double t1 = 1.0;
    double xdelta = x1src - x0src;
    double ydelta = y1src - y0src;
    double p, q, r;
    double edges[][2] = {
        {-xdelta, -(edgeLeft - x0src)},
        {xdelta, (edgeRight - x0src)},
        {-ydelta, -(edgeBottom - y0src)},
        {ydelta, (edgeTop - y0src)}
    };

    for (int edge = 0; edge < 4; edge++) 
    {
        // Traverse through left, right, bottom, top edges.
        p = edges[edge][0];
        q = edges[edge][1];
        r = q / p;
        if (p == 0 && q < 0) return false;   // Don't draw line at all. (parallel line outside)

        if (p < 0) 
        {
            if (r > t1) 
                return false;         // Don't draw line at all.
            else if (r > t0) 
                t0 = r;            // Line is clipped!
        } 
        else if (p > 0) 
        {
            if (r < t0) return false;      // Don't draw line at all.
            else if (r < t1) t1 = r;         // Line is clipped!
        }
    }

    x0clip = x0src + t0 * xdelta;
    y0clip = y0src + t0 * ydelta;
    x1clip = x0src + t1 * xdelta;
    y1clip = y0src + t1 * ydelta;

    return x0clip != x1clip || y0clip != y1clip;        // (clipped) line is drawn
}

// Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used, distributed and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// From http://geomalgorithms.com/a02-_lines.html

// Assume that classes are already given for the objects:
//     Point and Vector with
//          coordinates {float x, y, z;} (z=0  for 2D)
//          appropriate operators for:
//               Point  = Point ± Vector
//               Vector = Point - Point
//               Vector = Scalar * Vector
//     Line with defining endpoints {Point P0, P1;}
//     Segment with defining endpoints {Point P0, P1;}
//===================================================================


class Vector
{
public:
    double x, y, z;
};

Vector operator-(const Point &a, const Point &b)
{
    Vector v = {a.x - b.x, a.y - b.y, a.z - b.z};
    return v;
}

Vector operator*(double d, const Vector &a)
{
    Vector v = {a.x * d, a.y * d, a.z * d};
    return v;
}

Point operator+(const Point &a, const Vector &b)
{
    Point p = {a.x + b.x, a.y + b.y, a.z + b.z};
    return p;
}

class Line
{
public:
    Point P0, P1;
};

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm(v)     sqrtf((float) dot(v,v))     // norm = length of  vector
#define d(u,v)      norm(u-v)          // distance = norm of difference
 


// closest2D_Point_to_Line(): find the closest 2D Point to a Line
//     Input:  an array P[] of n points, and a Line L
//     Return: the index i of the Point P[i] closest to L
int
closest2D_Point_to_Line( Point P[], int n, Line L)
{
     // Get coefficients of the implicit line equation.
     // Do NOT normalize since scaling by a constant
     // is irrelevant for just comparing distances.
     float a = float(L.P0.y - L.P1.y);
     float b = float(L.P1.x - L.P0.x);
     float c = float(L.P0.x * L.P1.y - L.P1.x * L.P0.y);

     // initialize min index and distance to P[0]
     int mi = 0;
     float min = float(a * P[0].x + b * P[0].y + c);
     if (min < 0) min = -min;     // absolute value

     // loop through Point array testing for min distance to L
     for (int i=1; i<n; i++) {
          // just use dist squared (sqrt not  needed for comparison)
          float dist = float(a * P[i].x + b * P[i].y  + c);
          if (dist < 0) dist = -dist;    // absolute value
          if (dist < min) {      // this point is closer
               mi = i;              // so have a new minimum
               min = dist;
          }
     }
     return mi;     // the index of the closest  Point P[mi]
}
//===================================================================


// dist_Point_to_Line(): get the distance of a point to a line
//     Input:  a Point P and a Line L (in any dimension)
//     Return: the shortest distance from P to L
float
dist_Point_to_Line( Point P, Line L)
{
     Vector v = L.P1 - L.P0;
     Vector w = P - L.P0;

     double c1 = dot(w,v);
     double c2 = dot(v,v);
     double b = c1 / c2;

     Point Pb = L.P0 + b * v;
     return float(d(P, Pb));
}
//===================================================================


// dist_Point_to_Segment(): get the distance of a point to a segment
//     Input:  a Point P and a Segment S (in any dimension)
//     Return: the shortest distance from P to S
float dist_Point_to_Segment(const Point &P, const Segment &S)
{
     Vector v = S.P1 - S.P0;
     Vector w = P - S.P0;

     double c1 = dot(w,v);
     if ( c1 <= 0 )
          return float(d(P, S.P0));

     double c2 = dot(v,v);
     if ( c2 <= c1 )
          return float(d(P, S.P1));

     double b = c1 / c2;
     Point Pb = S.P0 + b * v;
     return float(d(P, Pb));
}
//===================================================================

}; // namespace Calculations
