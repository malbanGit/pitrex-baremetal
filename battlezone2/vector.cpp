#include "vector.h"
#include "mathutils.h"

#include "scene.h"
extern scene oglScene; // UGLY

static const float thicknessConstantLine = .0165 * .5;
//static const float thicknessConstantGlow = .0165 * 2; // 800x600
static const float thicknessConstantGlow = .014; // 1920 x 1080

static int glowCount=10;
static int lineCount=10;

namespace vector
{
    void drawVectorLine(const Point3d& from, const Point3d& to, const pen& penStyleFrom, const pen& penStyleTo)
    {
	    float r = theScene->mFlicker ? 1 : .85;

	    Point3d f(from);
	    Point3d t(to);
/*
	    glColor4f(penStyleFrom.r, penStyleFrom.g, penStyleFrom.b, penStyleFrom.a*r);
	    glVertex3f(f.x, f.y, 0 );

	    glColor4f(penStyleTo.r, penStyleTo.g, penStyleTo.b, penStyleTo.a*r);
	    glVertex3f(t.x, t.y, 0 );
*/	    

// actually "a" is somewhat like brightness in this context!
// e.g. decreases with distance!

float intensity=penStyleTo.r+penStyleTo.g+penStyleTo.b/3.0*127.0;
intensity = 90*(penStyleFrom.a);

drawLine(f.x, f.y, t.x, t.y, (int) intensity);
	    
    }

    void extendVector(Point3d* from, Point3d* to, double amount)
    {
        double angle = mathutils::calculateScreenAngle(*from, *to) - mathutils::DegreesToRads(90);
        float distance = mathutils::calculateScreenDistance(*from, *to) * amount;
        Point3d midPoint((from->x + to->x) / 2, (from->y + to->y) / 2, 0);

        Point3d vector1(0,distance/2,0);
        Point3d vector2(0,-distance/2,0);

        vector1 = mathutils::rotateScreenPoint(vector1, angle);
        vector2 = mathutils::rotateScreenPoint(vector2, angle);

        vector1 += midPoint;
        vector2 += midPoint;

        from->x = vector1.x;
        from->y = vector1.y;

        to->x = vector2.x;
        to->y = vector2.y;
    }

};

