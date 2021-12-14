#pragma once

#include "player.h"

class players
{
public:
	players();
	~players();

	void run();
	void draw();

	void startGame(int numPlayers);

	player mPlayer1;
};

