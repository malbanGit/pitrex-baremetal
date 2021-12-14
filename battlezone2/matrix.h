#ifndef MATRIX_H
#define MATRIX_H

#include "point3d.h"

class matrix
{
public:

    float _matrix[4][4];

    matrix () { Identity(); }

    matrix (const matrix& mat)
    {
        Copy(mat);
    }

    void Copy (const matrix& mat)
    {

        _matrix[0][0] = mat._matrix[0][0];  _matrix[0][1] = mat._matrix[0][1];  _matrix[0][2] = mat._matrix[0][2];  _matrix[0][3] = mat._matrix[0][3];
        _matrix[1][0] = mat._matrix[1][0];  _matrix[1][1] = mat._matrix[1][1];  _matrix[1][2] = mat._matrix[1][2];  _matrix[1][3] = mat._matrix[1][3];
        _matrix[2][0] = mat._matrix[2][0];  _matrix[2][1] = mat._matrix[2][1];  _matrix[2][2] = mat._matrix[2][2];  _matrix[2][3] = mat._matrix[2][3];
        _matrix[3][0] = mat._matrix[3][0];  _matrix[3][1] = mat._matrix[3][1];  _matrix[3][2] = mat._matrix[3][2];  _matrix[3][3] = mat._matrix[3][3];
    }


    inline void Identity()
    {
        _matrix[0][0]=1;  _matrix[0][1]=0;  _matrix[0][2]=0;  _matrix[0][3]=0;
        _matrix[1][0]=0;  _matrix[1][1]=1;  _matrix[1][2]=0;  _matrix[1][3]=0;
        _matrix[2][0]=0;  _matrix[2][1]=0;  _matrix[2][2]=1;  _matrix[2][3]=0;
        _matrix[3][0]=0;  _matrix[3][1]=0;  _matrix[3][2]=0;  _matrix[3][3]=1;
    }

    inline void Multiply (const matrix& mat)
    {
        int i,j;

        matrix temp(*this);

        for (i=0; i<4; i++)
        {
            for (j=0; j<4; j++)
            {
                _matrix[i][j] = ((temp._matrix[i][0] * mat._matrix[0][j])
                                 + (temp._matrix[i][1] * mat._matrix[1][j])
                                 + (temp._matrix[i][2] * mat._matrix[2][j])
                                 + (temp._matrix[i][3] * mat._matrix[3][j]));
            }
        }
    }

    inline void Scale (float sx, float sy, float sz)
    {
        _matrix[0][0] *= sx;  _matrix[1][1] *= sy;  _matrix[2][2] *= sz;
    }

    inline void Translate (float tx, float ty, float tz)
    {
        _matrix[3][0] += tx;  _matrix[3][1] += ty;  _matrix[3][2] += tz;
    }

    inline void Rotate (float rx, float ry, float rz)
    {
        matrix xmat, ymat, zmat;

        // x axis

        xmat._matrix[1][1]=cos(rx);  xmat._matrix[1][2]=sin(rx);
        xmat._matrix[2][0]=0;        xmat._matrix[2][1]=-sin(rx); xmat._matrix[2][2]=cos(rx);

        // y axis

        ymat._matrix[0][0]=cos(ry);  ymat._matrix[0][2]=-sin(ry);
        ymat._matrix[2][0]=sin(ry);  ymat._matrix[2][2]=cos(ry);

        // z axis

        zmat._matrix[0][0]=cos(rz);  zmat._matrix[0][1]=sin(rz);
        zmat._matrix[1][0]=-sin(rz); zmat._matrix[1][1]=cos(rz);

        matrix temp(xmat);
        temp.Multiply(ymat);
        temp.Multiply(zmat);

        Multiply(temp);
    }

    inline void TransformVertex (const Point3d &vertex, Point3d* result) const
    {
        float x = vertex.x * _matrix[0][0] + vertex.y * _matrix[1][0] + vertex.z * _matrix[2][0] + _matrix[3][0];
        float y = vertex.x * _matrix[0][1] + vertex.y * _matrix[1][1] + vertex.z * _matrix[2][1] + _matrix[3][1];
        float z = vertex.x * _matrix[0][2] + vertex.y * _matrix[1][2] + vertex.z * _matrix[2][2] + _matrix[3][2];

        result->x = x;
        result->y = y;
        result->z = z;
    }

    inline void ProjectVertex (Point3d &point, const Point3d &vertex) const
    {
		const float distance = 2;

        // avoid divide by zero error...
        float z = vertex.z;
        if (z == 0) z = 0.00001;

        // project
        point.x = distance * (vertex.x) / z;
        point.y = distance * (vertex.y) / z;
		point.z = vertex.z;
    }

	inline Point3d WorldToScreen (const Point3d &vertex) const
	{
		Point3d spoint;
		Point3d tpoint;
		TransformVertex(vertex, &tpoint);
		ProjectVertex(spoint, tpoint);
		return spoint;
	}

};

#endif // MATRIX_H
