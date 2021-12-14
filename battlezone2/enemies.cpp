#include "enemies.h"
#include "game.h"
#include "entity.h"

#include "entityTank.h"
#include "entitySupertank.h"
#include "entitySaucer.h"
#include "entityMissile.h"

enemies::enemies()
{
	int i = 0;

    // Tank
    mEnemies[i++] = new entityTank();

    // Supertank
    mEnemies[i++] = new entitySupertank();

	// Saucer
    mEnemies[i++] = new entitySaucer();

	// Missile
    mEnemies[i++] = new entityMissile();

    // Fill the rest of the list with empty entries
    for (int j=i; j<NUM_ENEMIES; j++)
    {
		mEnemies[j] = static_cast<enemy*>(entity::createEntity(entity::ENTITY_TYPE_UNDEF));
    }
}

enemies::~enemies()
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
		delete mEnemies[i];
    }
}

void enemies::run()
{
    // Run each enemy
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        switch(mEnemies[i]->getState())
        {
            case entity::ENTITY_STATE_SPAWN_TRANSITION:
                mEnemies[i]->spawnTransition();
                break;
            case entity::ENTITY_STATE_SPAWNING:
                mEnemies[i]->spawn();
                break;
            case entity::ENTITY_STATE_RUN_TRANSITION:
                mEnemies[i]->runTransition();
                break;
            case entity::ENTITY_STATE_RUNNING:
                mEnemies[i]->run();
                break;
            case entity::ENTITY_STATE_DESTROY_TRANSITION:
                mEnemies[i]->destroyTransition();
                break;
            case entity::ENTITY_STATE_DESTROYED:
                mEnemies[i]->destroy();
                break;
        }
    }

	mWreckage.run();
}

void enemies::draw(const camera& cam)
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if (mEnemies[i]->getEnabled())
        {
            mEnemies[i]->draw(cam);
        }
    }

	mWreckage.draw(cam);
}

enemy* enemies::hitTestEnemiesAtPoint(const Point3d& point, float radius)
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if (mEnemies[i]->getState() == entity::ENTITY_STATE_RUNNING)
        {
            enemy* e = static_cast<enemy*>(mEnemies[i]->hitTest(point, radius));
            if (e)
            {
                return e;
            }
        }
    }

    return NULL;
}

entity* enemies::getActiveEnemy()
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if ((mEnemies[i]->getState() == entity::ENTITY_STATE_RUNNING)
			&& ((mEnemies[i]->getType() == entity::ENTITY_TYPE_TANK) || (mEnemies[i]->getType() == entity::ENTITY_TYPE_SUPERTANK) || (mEnemies[i]->getType() == entity::ENTITY_TYPE_MISSILE)))
        {
            return mEnemies[i];
        }
    }

	return NULL;
}

int enemies::getNumActiveEnemiesOfType(const entity::EntityType& type)
{
    int count = 0;
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if (mEnemies[i]->getType() == type && mEnemies[i]->getState() != entity::ENTITY_STATE_INACTIVE)
        {
            ++count;
        }
    }

    return count;
}

entity* enemies::getUnusedEnemyOfType(const entity::EntityType& type)
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if (mEnemies[i]->getType() == type && mEnemies[i]->getState() == entity::ENTITY_STATE_INACTIVE)
        {
            return mEnemies[i];
        }
    }

    return NULL;
}

void enemies::disableAllEnemies()
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        mEnemies[i]->setState(entity::ENTITY_STATE_INACTIVE);
    }

	game::mSound.stopTrack(SOUNDID_MISSILE_LOOP);
	game::mSound.stopTrack(SOUNDID_SAUCER_LOOP);
}

void enemies::explodeEnemy(enemy* e)
{
	mWreckage.explodeTank(*e, e->getPos(), e->getHeading());
}

void enemies::initMissilePatterns()
{
    for (int i=0; i<NUM_ENEMIES; i++)
    {
        if (mEnemies[i]->getType() == entity::ENTITY_TYPE_MISSILE)
        {
            entityMissile* missile = static_cast<entityMissile*>(mEnemies[i]);
            missile->mPatternIndex = 0;
        }
    }

    
}