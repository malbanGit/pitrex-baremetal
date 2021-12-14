#ifndef _OBJ_LOADER_H
#define _OBJ_LOADER_H

#include <math.h>
#include "model.h"

class objloader
{
public:
	objloader() {;}
    
    static model* loadFile(char* filename);

};

#endif
