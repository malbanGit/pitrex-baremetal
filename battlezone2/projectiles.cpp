#include "projectiles.h"
#include "game.h"
#include "entityObstruction.h"

projectiles::projectiles()
{
    for (int i=0; i<NUM_PROJECTILES; i++)
    {
		mProjectiles[i] = dynamic_cast<entityProjectile*>(entity::createEntity(entity::ENTITY_TYPE_PROJECTILE));
    }
}

projectiles::~projectiles()
{
    for (int i=0; i<NUM_PROJECTILES; i++)
    {
		delete mProjectiles[i];
    }
}

void projectiles::run()
{
    for (int i=0; i<NUM_PROJECTILES; i++)
    {
		if (mProjectiles[i])
		{
			switch (mProjectiles[i]->getState())
			{
				case entity::ENTITY_STATE_DESTROY_TRANSITION:
					mProjectiles[i]->destroyTransition();
					break;
				case entity::ENTITY_STATE_DESTROYED:
					mProjectiles[i]->destroy();
					break;
				case entity::ENTITY_STATE_INACTIVE:
					break;
				default:
					{
						++mProjectiles[i]->mDurationTimer;
						if (mProjectiles[i]->mDurationTimer > 140)
						{
							mProjectiles[i]->setState(entity::ENTITY_STATE_DESTROY_TRANSITION);
						}
						else
						{
							mProjectiles[i]->run();
						}

						// Hit test against obstacles
						{
							entity* e = game::mObstructions.hitTestObstructionsAtPoint(mProjectiles[i]->getPos());
							entityObstruction* obstruction = dynamic_cast<entityObstruction*>(e);
							if (obstruction)
							{
								if (obstruction->mObstructsFire)
								{
									game::mSound.playTrack(SOUNDID_PROJECTILE_BLOCKED);

									// Destroy the projectile
									mProjectiles[i]->setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

									// Throw out some particles
									Point3d pos(mProjectiles[i]->getPos().inverse());
									Point3d angle(0,0,0);
									float speed = .6;
									float spread = 2*PI;
									int num = 50;
									int timeToLive = 300;
									vector::pen pen = mProjectiles[i]->getPen();
									pen.a = 1;
									pen.lineRadius=4;
									game::mParticles.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, FALSE, .99, FALSE);

								}
							}
						}

						// Hit test against enemies
						if (mProjectiles[i]->mSource != entityProjectile::PROJECTILE_SOURCE_ENEMY)
						{
							enemy* e = game::mEnemies.hitTestEnemiesAtPoint(mProjectiles[i]->getPos());
							if (e)
							{
			                    game::mSound.playTrack(SOUNDID_ENEMY_EXPLODE);

								// Destroy the projectile
								mProjectiles[i]->setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

								// Explode wreckage
								if (e->getType() != entity::ENTITY_TYPE_SAUCER)
								{
									game::mEnemies.explodeEnemy(e);
								}

								// Destroy the enemy
								e->setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

								// Increment the kill counter
								game::mSpawner.addKill();

								// Add points
								game::mPlayers.mPlayer1.addPoints(e->mPointValue);

								// Throw out some particles
								Point3d pos(mProjectiles[i]->getPos().inverse());
								Point3d angle(0,0,0);
								float speed = .6;
								float spread = 2*PI;
								int num = 100;
								int timeToLive = 300;
								vector::pen pen = mProjectiles[i]->getPen();
								pen.a = 10;
								pen.lineRadius=4;
								game::mParticles.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, FALSE, .99, FALSE);

							}
						}

						// Hit test against the player
						if (mProjectiles[i]->mSource == entityProjectile::PROJECTILE_SOURCE_ENEMY)
						{
							float distance = mathutils::calculate2dDistance(mProjectiles[i]->getPos(), game::mPlayers.mPlayer1.getPos());
							if (distance < 4)
							{
			                    game::mSound.playTrack(SOUNDID_PLAYER_EXPLODE);

								// Destroy the projectile
								mProjectiles[i]->setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

								// Destroy the player
								game::mPlayers.mPlayer1.setState(entity::ENTITY_STATE_DESTROY_TRANSITION);

								// Throw out some particles
								Point3d pos(mProjectiles[i]->getPos().inverse());
								Point3d angle(0,0,0);
								float speed = .6;
								float spread = 2*PI;
								int num = 100;
								int timeToLive = 300;
								vector::pen pen = mProjectiles[i]->getPen();
								pen.a = 1;
								pen.lineRadius=4;
								game::mParticles.emitter(&pos, &angle, speed, spread, num, &pen, timeToLive, FALSE, .99, FALSE);

							}
						}

					}
					break;
			}
		}
    }
}

void projectiles::draw(const camera& cam)
{
    for (int i=0; i<NUM_PROJECTILES; i++)
    {
		if (mProjectiles[i] && (mProjectiles[i]->getState() == entity::ENTITY_STATE_RUNNING))
		{
			mProjectiles[i]->draw(cam);
		}
    }
}

entityProjectile* projectiles::getUnusedProjectile()
{
    for (int i=0; i<NUM_PROJECTILES; i++)
    {
		if (mProjectiles[i] && (mProjectiles[i]->getState() == entity::ENTITY_STATE_INACTIVE))
		{
			return mProjectiles[i];
		}
    }

	return NULL;
}



