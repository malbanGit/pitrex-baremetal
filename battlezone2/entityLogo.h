#pragma once

#include "entity.h"

class entityLogo :
	public entity
{
public:
	entityLogo();
	~entityLogo();

    virtual void run();
    virtual void draw(const camera& cam);

};

