#include "players.h"
#include "game.h"


players::players()
{
}

players::~players()
{
}

void players::run()
{
	if (game::mGameMode != game::GAMEMODE_PLAYING)
	{
		// For attract mode
		mPlayer1.run();
	}
	else
	{
		switch(mPlayer1.getState())
		{
			case entity::ENTITY_STATE_SPAWN_TRANSITION:
				mPlayer1.spawnTransition();
				break;
			case entity::ENTITY_STATE_SPAWNING:
				mPlayer1.spawn();
				break;
			case entity::ENTITY_STATE_RUN_TRANSITION:
				mPlayer1.runTransition();
				break;
			case entity::ENTITY_STATE_RUNNING:
				mPlayer1.run();
				break;
			case entity::ENTITY_STATE_DESTROY_TRANSITION:
				mPlayer1.destroyTransition();
				break;
			case entity::ENTITY_STATE_DESTROYED:
				mPlayer1.destroy();
				break;
		}
	}
}

void players::draw()
{
	mPlayer1.draw();
}

void players::startGame(int numPlayers)
{
	mPlayer1.startGame();
}

