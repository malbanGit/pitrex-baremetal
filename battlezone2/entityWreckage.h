#pragma once

#include "entity.h"

class entityWreckage :
	public entity
{
public:
	entityWreckage();
	~entityWreckage();

	int mFadeTimer;
};

