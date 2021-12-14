#include "objloader.h"
#include <vector>

model* objloader::loadFile(char* filename)
{
    printf((char *)"objloader::loadFile(%s)\n", filename);
    
    if (filename && strlen(filename))
    {
		FILE* file = fopen(filename, (char *) "r");
		if (file)
		{
			std::vector<int> lineList;
			std::vector<Point3d*> vertexArray;
			std::vector<std::vector<int>> faceArray;

			char* buffer = new char[1000];

			int i=0;
			while (fgets (buffer, 1000, file) != NULL )
			{
				// remove the trailing newline
				int len = strlen(buffer);
				if( buffer[len-1] == '\n' )
					buffer[len-1] = 0;

				// Check for empty lines or comments
				if ((strlen(buffer) == 0) || (buffer[0] == '#'))
					continue;

				// Tokenize this line
				std::vector<char*> tokens;
				char* token = strtok(buffer, " ");
				while(token != NULL)
				{
					char* tokenCopy = new char[256];
					strcpy(tokenCopy, token);
					tokens.push_back(tokenCopy);

					// Continue tokeninzing
					token = strtok(NULL, (char *) " ");
				};

				if (tokens.size() > 0)
				{
					// Parse this line
					if (strcmp(tokens[0], (char *) "v") == 0)
					{
						// It's a vertex
						float x = strtod(tokens[1], NULL);
						float y = strtod(tokens[2], NULL);
						float z = strtod(tokens[3], NULL);

						// Add the point to the list
						Point3d* point = new Point3d(x, y, z);
						vertexArray.push_back(point);
					}
					else if (strcmp(tokens[0], (char *) "f") == 0)
					{
						std::vector<int> faceIndicies;

						int size = tokens.size();
						for (int i=1; i<size; i++)
						{
							// For now we only care about the vertex value (ignoring the optional texture coords and normals)
							char* faceIndex = strtok(tokens[i], (char *) "/");
							int index = strtol(faceIndex, NULL, 10)-1; // convert to 0-based index
							faceIndicies.push_back(index);
						}

						// Add the list of vertex indexes to the face list
						faceArray.push_back(faceIndicies);
					}
				}


				// Delete stuff
				int numTokens = tokens.size();
				for (int i=0; i<numTokens; i++)
				{
					delete tokens[i];
				}
			};


			int numVertex = vertexArray.size();
			int numFaces = faceArray.size();

			TCHAR s[256];
			printf( (char *) "Found %d Vertexes\n", numVertex);

			printf((char *) "Found %d Faces\n", numFaces);

			delete buffer;
			fclose(file);


			// Create a model with the data we just loaded
			model* object = new model();
			if (object)
			{
				object->mNumVertex = numVertex;
				object->mVertexList = new Point3d[object->mNumVertex];
                
				for (int i=0; i<numVertex; i++)
				{
					Point3d* point = vertexArray[i];
                    
					object->mVertexList[i].x = point->z;
					object->mVertexList[i].y = point->y;
					object->mVertexList[i].z = point->x;

					delete point;
				}
// for vectrex
// This display is TERRIBLE
// each face is drawn with its vertices
// for the logo, thus 576 lines are drawn (or even points)
// removing the double, inverse double, and points at line ends
// reduces that to 72 Vectors!

// still I am not doing that here... I will do a cleanup in the vectrex pipeline later on.

				object->mNumFaces = numFaces;
				object->mFaceList = new model::face[object->mNumFaces];
                
				for (int i=0; i<numFaces; i++)
				{
					std::vector<int> vlist = faceArray[i];

					int numVerticies = vlist.size();

					object->mFaceList[i].numVerticies = numVerticies;
					object->mFaceList[i].verticies = new int[numVerticies];
                    
					for (int j=0; j<numVerticies; ++j)
					{
						int v = vlist[j];
						object->mFaceList[i].verticies[j] = v;
					}
				}

				printf((char *) "Model loaded and created successfully\n");
			}
            
			return object;
		}
		else
		{
			printf((char *) "File not found\n");
		}
    }
    else
    {
        printf((char *) "ObjLoader::loadFile() - bad filename\n");
    }
    
    return NULL;
}

