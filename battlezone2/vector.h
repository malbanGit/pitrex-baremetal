#ifndef VECTOR_H
#define VECTOR_H

#include "defines.h"
#include "point3d.h"

//#define DRAW_BW

namespace vector
{
	class pen
	{
	    public:
	      pen() {r=g=b=a=1; lineRadius=1;}
	      pen(float _r, float _g, float _b, float _a, float _radius) { r=_r; g=_g, b=_b; a=_a; lineRadius=_radius; }
	      pen(const pen& p, float _a, float _radius) { r=p.r; g=p.g, b=p.b; a=_a; lineRadius=_radius; }
	      float r,g,b,a;
	      float lineRadius;
	};

	static pen penGreen()
	{
#ifdef DRAW_BW
		return vector::pen(.5,.5,.5,.2,3);
#else
		return vector::pen(.5,.8,.5,.2,3);
#endif
	}
	static pen penRed()
	{
#ifdef DRAW_BW
		return vector::pen(.5,.5,.5,.5,3);
#else
		return vector::pen(.8,.5,.5,.5,3);
#endif
	}
	static pen penFaintWhite()
	{
		return vector::pen(.5,.5,.5,.2,3);
	}

	void drawVectorLine(const Point3d& from, const Point3d& to, const pen& penStyleFrom, const pen& penStyleTo);
	void extendVector(Point3d* from, Point3d* to, double amount);
};


#endif // VECTOR_H
