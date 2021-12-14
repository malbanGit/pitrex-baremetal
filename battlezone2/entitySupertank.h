#pragma once

#include "entityTank.h"

class entitySupertank :
	public entityTank
{
public:
	entitySupertank();
	~entitySupertank();

    virtual void draw(const camera& cam);

};

