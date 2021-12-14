#ifndef MODEL_H
#define MODEL_H

#include "point3d.h"
#include "vector.h"
#include "matrix.h"
#include "camera.h"

class model
{
public:

    typedef struct
    {
	    int 	numVerticies;
	    int*	verticies;
    } face;

    model();

    void draw(const camera& camera, const vector::pen& pen);

    void Identity() { mMatrix.Identity(); }
    void Scale(Point3d scale) { mMatrix.Scale(scale.x, scale.y, scale.z); }
    void Translate(Point3d trans) { mMatrix.Translate(trans.x, trans.y, trans.z); }
    void Rotate(float angle) { mMatrix.Rotate(0, 0, angle); }
    void Rotate(float x, float y, float z) { mMatrix.Rotate(x, y, z); }

    float getBottom();

    static void flipXY(model* m);

    Point3d* mVertexList;
    face* mFaceList;

    int mNumVertex;
    int mNumFaces;

    matrix mMatrix;
};

#endif // MODEL_H
