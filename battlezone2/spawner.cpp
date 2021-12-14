#include "spawner.h"
#include "game.h"

spawner::spawner()
{
	reset();
}

spawner::~spawner()
{
}

void spawner::reset()
{
	mKillCount = 0;
	mMissileVolleyCount = 0;
}

void spawner::addKill()
{
	++mKillCount;
}

void spawner::run()
{
	// Players

	if (game::mPlayers.mPlayer1.getState() == entity::ENTITY_STATE_INACTIVE)
	{
		game::mPlayers.mPlayer1.setState(entity::ENTITY_STATE_SPAWN_TRANSITION);
	}

	if ((game::mPlayers.mPlayer1.getState() != entity::ENTITY_STATE_RUNNING)
		&& (game::mGameMode == game::GAMEMODE_PLAYING))
		return;


	// Enemies

	BOOL wantTank = FALSE;
	BOOL wantSupertank = FALSE;
	BOOL wantMissile = FALSE;

	int numTanks = game::mEnemies.getNumActiveEnemiesOfType(entity::ENTITY_TYPE_TANK);
	int numSupertanks = game::mEnemies.getNumActiveEnemiesOfType(entity::ENTITY_TYPE_SUPERTANK);
	int numMissiles = game::mEnemies.getNumActiveEnemiesOfType(entity::ENTITY_TYPE_MISSILE);
	
	if (numTanks + numSupertanks == 0)
	{
		if (mMissileVolleyCount == 0)
		{
			// Missiles start at 10000 points
			if (game::mPlayers.mPlayer1.mPoints >= 10000)
			{
				if (mathutils::frandFrom0To1() * 100 < 40)
				{
					mMissileVolleyCount = (int)(mathutils::frandFrom0To1() * 3);
				}
			}
		}

		if (mMissileVolleyCount)
		{
			wantMissile = TRUE;
			wantTank = FALSE;
			wantSupertank = FALSE;
		}
		else
		{
			if (mKillCount < 10)
			{
				wantTank = TRUE;
				wantSupertank = FALSE;
			}
			else
			{
				if (mathutils::frandFrom0To1() * 10 < 2)
				{
					wantTank = TRUE;
					wantSupertank = FALSE;
				}
				else
				{
					wantTank = FALSE;
					wantSupertank = TRUE;
				}
			}
		}
	}

	if (wantTank && (numMissiles == 0))
	{
		entity* e = game::mEnemies.getUnusedEnemyOfType(entity::ENTITY_TYPE_TANK);
		if (e)
		{
			if (game::mGameMode == game::GAMEMODE_PLAYING)
			{
				game::mSound.playTrack(SOUNDID_NEW_ENEMY);
			}
			e->setState(entity::ENTITY_STATE_SPAWN_TRANSITION);
		}
	}

	if (wantSupertank && (numMissiles == 0))
	{
		entity* e = game::mEnemies.getUnusedEnemyOfType(entity::ENTITY_TYPE_SUPERTANK);
		if (e)
		{
			if (game::mGameMode == game::GAMEMODE_PLAYING)
			{
				game::mSound.playTrack(SOUNDID_NEW_ENEMY);
			}
			e->setState(entity::ENTITY_STATE_SPAWN_TRANSITION);
		}
	}

	if (game::mGameMode == game::GAMEMODE_PLAYING)
	{
		if (wantMissile && (mMissileVolleyCount > 0) && (numTanks + numSupertanks == 0))
		{
			--mMissileVolleyCount;
			if (game::mPlayers.mPlayer1.getState() == entity::ENTITY_STATE_RUNNING)
			{
				if (game::mEnemies.getNumActiveEnemiesOfType(entity::ENTITY_TYPE_MISSILE) == 0)
				{
					entity* e = game::mEnemies.getUnusedEnemyOfType(entity::ENTITY_TYPE_MISSILE);
					if (e)
					{
						e->setState(entity::ENTITY_STATE_SPAWN_TRANSITION);
					}
				}
			}
		}

		if (game::mEnemies.getNumActiveEnemiesOfType(entity::ENTITY_TYPE_SAUCER) == 0)
		{
			entity* e = game::mEnemies.getUnusedEnemyOfType(entity::ENTITY_TYPE_SAUCER);
			if (e)
			{
				e->setState(entity::ENTITY_STATE_SPAWN_TRANSITION);
			}
		}
	}
}

