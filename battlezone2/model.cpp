#include "model.h"
#include "mathutils.h"

#define _MAX_DISTANCE 500

model::model()
{
    mMatrix.Identity();

    mNumVertex = 0;
    mNumFaces = 0;
    mVertexList = NULL;
    mFaceList = NULL;
}

void model::draw(const camera& camera, const vector::pen& pen)
{
	// Combine the camera's matrix with the object's local matrix
	matrix matrix(mMatrix);
	matrix.Multiply(camera.mMatrix);

	static int allFromX[1000];
	static int allFromY[1000];
	static int allFromZ[1000];
	static int allToX[1000];
	static int allToY[1000];
	static int allToZ[1000];
	int allCount = 0;
	
	
	// For each face
	for (int f=0; f<mNumFaces; f++)
	{
		// For each vertex of the face
		face theface = mFaceList[f];

		Point3d vertexFrom = mVertexList[theface.verticies[0]];

		for (int v=1; v<theface.numVerticies+1; v++)
		{
			Point3d vertexTo = mVertexList[theface.verticies[v % theface.numVerticies]];

			
			/////////////////////////////////////
			// Check/remove double draws within one model
			
			// in general forbid POINTS in a model
			int allow = !(((int) (V_MUL*vertexTo.x) == (int) (V_MUL*vertexFrom.x)) && ((int) (V_MUL*vertexTo.y) == (int) (V_MUL*vertexFrom.y)) && ((int) (V_MUL*vertexTo.z) == (int) (V_MUL*vertexFrom.z)) );
			
			if (allow)
			{
			  for (int i=0;i<allCount;i++)
			  {
				if (		(allFromX[i] == (int) (V_MUL*vertexFrom.x)) && 
					        (allFromY[i] == (int) (V_MUL*vertexFrom.y)) && 
					        (allFromZ[i] == (int) (V_MUL*vertexFrom.z)) && 
						(allToX[i] == (int) (V_MUL*vertexTo.x)) && 
						(allToY[i] == (int) (V_MUL*vertexTo.y)) && 
						(allToZ[i] == (int) (V_MUL*vertexTo.z))
				   )
				{
				  allow = false;
				  break;
				}
				// inverse vectors
				if (		(allFromX[i] == (int) (V_MUL*vertexTo.x)) && 
						(allFromY[i] == (int) (V_MUL*vertexTo.y)) && 
						(allFromZ[i] == (int) (V_MUL*vertexTo.z)) && 
						(allToX[i] == (int) (V_MUL*vertexFrom.x)) && 
						(allToY[i] == (int) (V_MUL*vertexFrom.y)) && 
						(allToZ[i] == (int) (V_MUL*vertexFrom.z))
				   )
				{
				  allow = false;
				  break;
				}
			  }
			}
			/////////////////////////////////////
			if (!allow) 
			{
			  vertexFrom = vertexTo;
			  continue;
			}
			
			
			allFromX[allCount] = (int) (V_MUL*vertexFrom.x);
			allFromY[allCount] = (int) (V_MUL*vertexFrom.y);
			allFromZ[allCount] = (int) (V_MUL*vertexFrom.z);
			allToX[allCount] = (int) (V_MUL*vertexTo.x);
			allToY[allCount] = (int) (V_MUL*vertexTo.y);
			allToZ[allCount] = (int) (V_MUL*vertexTo.z);
			allCount++;
			
			Point3d screenPointFrom = matrix.WorldToScreen(vertexFrom);
			Point3d screenPointTo = matrix.WorldToScreen(vertexTo);

			if ((screenPointFrom.z > .1) && (screenPointTo.z > .1) && (screenPointFrom.z < _MAX_DISTANCE) && (screenPointTo.z < _MAX_DISTANCE))
			{
				if (((screenPointFrom.x >= -1) && (screenPointFrom.x <= 1)) || ((screenPointTo.x >= -1) && (screenPointTo.x <= 1)))
				{
					float afrom = pen.a * (1-(screenPointFrom.z / _MAX_DISTANCE));
					float ato = pen.a * (1-(screenPointTo.z / _MAX_DISTANCE));

					vector::pen penFrom(pen);
					vector::pen penTo(pen);

					penFrom.a = afrom;
					penTo.a = ato;

					
					vector::drawVectorLine(screenPointFrom, screenPointTo, penFrom, penTo);
				}
			}

			vertexFrom = vertexTo;
		}
	}
}

void model::flipXY(model* m)
{
/*
	for (int i=0; i<m->mNumVertex; i++)
	{
		float temp = m->mVertexList[i].x;
		m->mVertexList[i].x = m->mVertexList[i].y;
		m->mVertexList[i].y = temp;
	}
*/
}

float model::getBottom()
{
	float min = 32000;

	for (int i=0; i<mNumVertex; i++)
	{
		if (mVertexList[i].y < min)
			min = mVertexList[i].y;
	}

	return min;
}




